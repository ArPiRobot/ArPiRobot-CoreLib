/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#include <arpirobot/core/network/NetworkManager.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <arpirobot/core/robot/BaseRobot.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>


using namespace arpirobot;
using namespace std::placeholders;


// Commands from drive station
const std::string arpirobot::COMMAND_ENABLE = "ENABLE";
const std::string arpirobot::COMMAND_DISABLE = "DISABLE";
const std::string arpirobot::COMMAND_NET_TABLE_SYNC = "NT_SYNC";

// Pre-defined (special) data packets
const uint8_t arpirobot::NET_TABLE_START_SYNC_DATA[] = {255, 255, '\n'};
const std::string arpirobot::NET_TABLE_END_SYNC_DATA = "\377\377\377\n";

// Static variables for NetworkManager
std::thread *NetworkManager::networkThread = nullptr;
bool NetworkManager::isDsConnected = false;
bool NetworkManager::networkingStarted = false;
std::array<uint8_t, 32> NetworkManager::tmpCommandRxBuf;
std::array<uint8_t, 32> NetworkManager::tmpNetTableRxBuf;
std::array<uint8_t, 32> NetworkManager::tmpControllerRxBuf;
std::vector<uint8_t> NetworkManager::commandRxData;
std::vector<uint8_t> NetworkManager::netTableRxData;
io_service NetworkManager::io;
io_service::work NetworkManager::wk(NetworkManager::io);
udp::socket NetworkManager::controllerSocket(NetworkManager::io, udp::endpoint(udp::v4(), 8090));
tcp::acceptor NetworkManager::commandSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8091));
tcp::acceptor NetworkManager::netTableSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8092));
tcp::acceptor NetworkManager::logSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8093));
udp::endpoint NetworkManager::controllerDataEndpoint;
tcp::socket NetworkManager::commandClient(NetworkManager::io);
tcp::socket NetworkManager::netTableClient(NetworkManager::io);
tcp::socket NetworkManager::logClient(NetworkManager::io);
std::function<void()> NetworkManager::enableFunc;
std::function<void()> NetworkManager::disableFunc;
std::unordered_map<std::string, std::string> NetworkManager::ntSyncData;
std::unordered_map<int, std::shared_ptr<ControllerData>> NetworkManager::controllerData;
MainVmon *NetworkManager::mainVmon = nullptr;

void NetworkManager::startNetworking(std::function<void()> enableFunc, std::function<void()> disableFunc){
    if(!networkingStarted){
        if(networkThread != nullptr){
            networkThread->join();
            delete networkThread;
        }

        // Store enable and disable function "pointers"
        NetworkManager::enableFunc = enableFunc;
        NetworkManager::disableFunc = disableFunc;

        // Wait for connection from drive station
        commandSocketAcceptor.async_accept(commandClient, std::bind(&NetworkManager::handleAccept, 
            std::ref(commandClient), _1));
        netTableSocketAcceptor.async_accept(netTableClient, std::bind(&NetworkManager::handleAccept, 
            std::ref(netTableClient), _1));
        logSocketAcceptor.async_accept(logClient, std::bind(&NetworkManager::handleAccept, 
            std::ref(logClient), _1));

        networkThread = new std::thread(&NetworkManager::runNetworking);

        Logger::logDebug("Starting Networking");
    }
}

void NetworkManager::stopNetworking(){
    io.stop();
}

bool NetworkManager::sendNtRaw(const_buffer buffer){
    if(isDsConnected){
        try{
            asio::write(netTableClient, buffer);
            return true;
        }catch(const std::system_error &err){
            if(err.code() == asio::error::eof || err.code() == asio::error::connection_reset){
                handleDisconnect(netTableClient);
            }
            return false;
        }
    }
    return false;
}

bool NetworkManager::sendNt(std::string key, std::string value){
    std::vector<uint8_t> data;
    for(size_t i = 0; i < key.length(); ++i){
        data.push_back(key[i]);
    }
    data.push_back(255);
    for(size_t i = 0; i < value.length(); ++i){
        data.push_back(value[i]);
    }
    data.push_back('\n');
    return sendNtRaw(asio::buffer(data));
}

void NetworkManager::sendLogMessage(std::string message){
    if(isDsConnected){
        try{
            asio::write(logClient, asio::buffer(message));
        }catch(const std::system_error &err){
            if(err.code() == asio::error::eof || err.code() == asio::error::connection_reset){
                handleDisconnect(logClient);
            }
        }
    }
}

void NetworkManager::setMainVmon(MainVmon *vmon){
    mainVmon = vmon;
}

bool NetworkManager::isMainVmon(MainVmon *vmon){
    return vmon == mainVmon;
}

void NetworkManager::sendMainBatteryVoltage(double voltage){
    std::stringstream vstr;
    vstr << std::fixed << std::setprecision(2) << voltage;
    NetworkTable::setFromRobot("vbat0", vstr.str());
}

void NetworkManager::runNetworking(){
    io.run();
}

void NetworkManager::receiveFrom(const tcp::socket &client){
    if(&client == &commandClient){
        commandClient.async_receive(asio::buffer(tmpCommandRxBuf), 
            std::bind(&NetworkManager::handleTcpReceive, std::ref(commandClient), _1, _2));
    }else if(&client == &netTableClient){
        netTableClient.async_receive(asio::buffer(tmpNetTableRxBuf), 
            std::bind(&NetworkManager::handleTcpReceive, std::ref(netTableClient), _1, _2));
    }
    // Data will never be received from log client
}

void NetworkManager::handleConnectionStatusChanged(){
    if(commandClient.is_open() && netTableClient.is_open() && logClient.is_open()){
        // All 3 connected
        std::string cmdAddress = commandClient.remote_endpoint().address().to_string();
        std::string netTableAddress = netTableClient.remote_endpoint().address().to_string();
        std::string logAddress = logClient.remote_endpoint().address().to_string();
        if(cmdAddress == netTableAddress && cmdAddress == logAddress){
            // Same address, valid DS
            isDsConnected = true;
            Logger::logInfo("Drive station connected.");
            
            // Start waiting for controller data
            controllerSocket.async_receive_from(asio::buffer(tmpControllerRxBuf), 
                controllerDataEndpoint, std::bind(&NetworkManager::handleUdpReceive, _1, _2));

        }else{
            // Multiple DS connections. Reject all
            Logger::logError("Partial drive station connection from multiple addresses. All have been disconnected.");
            commandClient.close();
            netTableClient.close();
            logClient.close();
            handleConnectionStatusChanged(); // So disconnect is handled properly
        }
    }else if (isDsConnected){
        // Handle disconnect
        Logger::logInfo("Drive station disconnected.");

        if(NetworkTable::isInSync()){
            NetworkTable::abortSync();
            Logger::logDebug("Net table sync aborted due to drive station disconnect.");
        }

        // Clear read buffers
        netTableRxData.clear();
        commandRxData.clear();

        isDsConnected = false;

        disableFunc();
    }
}

void NetworkManager::handleAccept(const tcp::socket &client, const std::error_code &ec){
    if(!ec){
        handleConnectionStatusChanged();
        receiveFrom(client); // Start receive now so disconnect will be detected
    }
}

void NetworkManager::handleDisconnect(const tcp::socket &client){
    commandClient.close();
    netTableClient.close();
    logClient.close();
    handleConnectionStatusChanged();

    commandSocketAcceptor.async_accept(commandClient, std::bind(&NetworkManager::handleAccept, 
        std::ref(commandClient), _1));
    netTableSocketAcceptor.async_accept(netTableClient, std::bind(&NetworkManager::handleAccept, 
        std::ref(netTableClient), _1));
    logSocketAcceptor.async_accept(logClient, std::bind(&NetworkManager::handleAccept, 
        std::ref(logClient), _1));

}

void NetworkManager::handleTcpReceive(const tcp::socket &client, 
        const std::error_code &ec, std::size_t count){
    if(!ec){
        if(count > 0 && isDsConnected){
            if(&client == &commandClient){
                for(int i = 0; i < count; ++i){
                    commandRxData.push_back(tmpCommandRxBuf[i]);
                }
                handleCommand();
            }else if(&client == &netTableClient){
                for(int i = 0; i < count; ++i){
                    netTableRxData.push_back(tmpNetTableRxBuf[i]);
                }
                handleNetTableData();
            }
        }

        // Wait for more data
        if(isDsConnected)
            receiveFrom(client);
    }else if(ec == asio::error::eof || ec == asio::error::connection_reset){
        handleDisconnect(client);
    }
}

void NetworkManager::handleUdpReceive(const std::error_code &ec, std::size_t count){
    bool shouldProcess = netTableClient.is_open() && commandClient.is_open() && logClient.is_open();
    if(!ec && shouldProcess){
        std::string controllerAddress = controllerDataEndpoint.address().to_string();
        std::string commandAddress = commandClient.remote_endpoint().address().to_string();
        if(controllerAddress == commandAddress){
            std::vector<uint8_t> data;
            data.reserve(count);
            for(size_t i = 0; i < count; ++i){
                data.push_back(tmpControllerRxBuf[i]);
            }
            handleControllerData(data);
        }
    }
    if(shouldProcess){
        controllerSocket.async_receive_from(asio::buffer(tmpControllerRxBuf), 
            controllerDataEndpoint, std::bind(&NetworkManager::handleUdpReceive, _1, _2));
    }
}

void NetworkManager::handleCommand(){

    auto startPos = commandRxData.begin();
    auto endPos = startPos - 1;

    // Could be multiple commands in the buffer, so handle all of them
    while(true){
        // End of last command is start of next
        startPos = endPos + 1;
        endPos = std::find(startPos, commandRxData.end(), '\n');
        if(endPos == commandRxData.end()){
            // Leave any incomplete data in the buffer
            commandRxData = std::vector<uint8_t>(startPos, commandRxData.end());
            break;
        }

        // Command excluding \n
        std::string subset = std::string(startPos, endPos);
        
        // Handle the command
        if(subset == COMMAND_ENABLE){
            Logger::logDebug("Got enable command");
            enableFunc();
        }else if(subset == COMMAND_DISABLE){
            Logger::logDebug("Got disable command");
            disableFunc();
        }else if(subset == COMMAND_NET_TABLE_SYNC){
            Logger::logDebug("Starting net table sync.");
            ntSyncData.clear();
            NetworkTable::startSync();
        }
    }
}

void NetworkManager::handleNetTableData(){
    auto startPos = netTableRxData.begin();
    auto endPos = startPos - 1;

    // Data can be split across multiple packets or multiple keys could be in one packet.
    // Handle any data in the buffer and leave what is not handled
    while(true){
        startPos = endPos + 1;
        endPos = std::find(startPos, netTableRxData.end(), '\n');
        if(endPos == netTableRxData.end()){
            // Leave any incomplete data in the buffer
            netTableRxData = std::vector<uint8_t>(startPos, netTableRxData.end());
            break;
        }

        // Subset until and including \n
        std::string subset = std::string(startPos, endPos + 1);

        // Handle data in the subset
        auto delim = subset.find((char)255);
        if(subset == NET_TABLE_END_SYNC_DATA){
            NetworkTable::finishSync(ntSyncData);
        }else if (delim != std::string::npos){
            std::string key = std::string(subset.begin(), subset.begin() + delim);
            std::string value = std::string(subset.begin() + delim + 1, subset.begin() + subset.length() - 1);
            if(NetworkTable::isInSync()){
                ntSyncData[key] = value;
            }else{
                NetworkTable::setFromDs(key, value);
            }
        }
    }
}

void NetworkManager::handleControllerData(std::vector<uint8_t> &data){
    // Only handle data that is long enough
    int l = data.size();
    if(l >= 4){
        int calcLen = 5 + 
            2 * data[1] + 
            (int)(std::ceil(data[2] / 8.0)) + 
            (int)(std::ceil(data[3] / 2.0));
        if(l == calcLen){
            // Correct amount of data in this packet. handle the data
            int controllerNum = data[0];
            if(controllerData.find(controllerNum) != controllerData.end()){
                controllerData[controllerNum]->updateData(data);
            }else{
                controllerData[controllerNum] = std::make_shared<ControllerData>(data);
            }
        }
    }
}