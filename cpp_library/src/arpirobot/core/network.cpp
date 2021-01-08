#include <arpirobot/core/network.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/robot.hpp>


// TODO: remove
#include <iostream>


using namespace arpirobot;


// Commands from drive station
const std::string arpirobot::COMMAND_ENABLE = "ENABLE";
const std::string arpirobot::COMMAND_DISABLE = "DISABLE";
const std::string arpirobot::COMMAND_NET_TABLE_SYNC = "NT_SYNC";

// Pre-defined (special) data packets
const uint8_t arpirobot::NET_TABLE_START_SYNC_DATA[] = {255, 255, '\n'};
const uint8_t arpirobot::NET_TABLE_END_SYNC_DATA[] = {255, 255, 255, '\n'};

// Static variables for NetworkManager
std::thread *NetworkManager::networkThread = nullptr;
bool NetworkManager::isDsConnected = false;
bool NetworkManager::networkingStarted = false;
std::array<uint8_t, 32> NetworkManager::tmpCommandRxBuf;
std::array<uint8_t, 32> NetworkManager::tmpNetTableRxBuf;
std::array<uint8_t, 32> NetworkManager::tmpControllerRxBuf;
std::vector<uint8_t> NetworkManager::commandRxData;
std::vector<uint8_t> NetworkManager::netTableRxData;
std::vector<uint8_t> NetworkManager::controllerRxData;
io_service NetworkManager::io;
io_service::work NetworkManager::wk(NetworkManager::io);
udp::socket NetworkManager::controllerSocket(NetworkManager::io, udp::endpoint(udp::v4(), 8090));
tcp::acceptor NetworkManager::commandSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8091));
tcp::acceptor NetworkManager::netTableSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8092));
tcp::acceptor NetworkManager::logSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8093));
tcp::socket NetworkManager::commandClient(NetworkManager::io);
tcp::socket NetworkManager::netTableClient(NetworkManager::io);
tcp::socket NetworkManager::logClient(NetworkManager::io);
std::function<void()> NetworkManager::enableFunc;
std::function<void()> NetworkManager::disableFunc;


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
        commandSocketAcceptor.async_accept(commandClient, boost::bind(&NetworkManager::handleAccept, 
            boost::ref(commandClient), _1));
        netTableSocketAcceptor.async_accept(netTableClient, boost::bind(&NetworkManager::handleAccept, 
            boost::ref(netTableClient), _1));
        logSocketAcceptor.async_accept(logClient, boost::bind(&NetworkManager::handleAccept, 
            boost::ref(logClient), _1));

        networkThread = new std::thread(&NetworkManager::runNetworking);

        Logger::logDebug("Starting Networking");
    }
}

void NetworkManager::stopNetworking(){
    io.stop();
}

void NetworkManager::runNetworking(){
    io.run();
}

void NetworkManager::receiveFrom(const tcp::socket &client){
    if(&client == &commandClient){
        commandClient.async_receive(boost::asio::buffer(tmpCommandRxBuf), 
            boost::bind(&NetworkManager::handleTcpReceive, boost::ref(commandClient), _1, _2));
    }else if(&client == &netTableClient){
        netTableClient.async_receive(boost::asio::buffer(tmpNetTableRxBuf), 
            boost::bind(&NetworkManager::handleTcpReceive, boost::ref(netTableClient), _1, _2));
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
            Logger::logInfo("Drive station connected.");
            isDsConnected = true;
            // TODO: Start receive UDP
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

        // TODO: Handle stop of network table sync

        // Clear read buffers
        controllerRxData.clear();
        netTableRxData.clear();
        commandRxData.clear();

        isDsConnected = false;

        disableFunc();
    }
}

void NetworkManager::handleAccept(const tcp::socket &client, const boost::system::error_code &ec){
    if(!ec){
        handleConnectionStatusChanged();
        receiveFrom(client); // Start receive now so disconnect will be detected
    }
}

void NetworkManager::handleDisconnect(const tcp::socket &client){
    commandClient.close();
    netTableClient.close();
    logClient.close();
    // TODO: Stop receiving from controller UDP port
    handleConnectionStatusChanged();

    commandSocketAcceptor.async_accept(commandClient, boost::bind(&NetworkManager::handleAccept, 
        boost::ref(commandClient), _1));
    netTableSocketAcceptor.async_accept(netTableClient, boost::bind(&NetworkManager::handleAccept, 
        boost::ref(netTableClient), _1));
    logSocketAcceptor.async_accept(logClient, boost::bind(&NetworkManager::handleAccept, 
        boost::ref(logClient), _1));

}

void NetworkManager::handleTcpReceive(const tcp::socket &client, 
        const boost::system::error_code &ec, std::size_t count){
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
            }
        }

        // Wait for more data
        receiveFrom(client);
    }else if(ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset){
        handleDisconnect(client);
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
            // TODO
        }
    }
}