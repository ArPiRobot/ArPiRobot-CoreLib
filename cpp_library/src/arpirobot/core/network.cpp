#include <arpirobot/core/network.hpp>
#include <arpirobot/core/log.hpp>
#include <arpirobot/core/robot.hpp>


using namespace arpirobot;


// Commands from drive station
const std::string arpirobot::COMMAND_ENABLE = "ENABLE";
const std::string arpirobot::COMMAND_DISABLE = "DISABLE";
const std::string arpirobot::COMMAND_NET_TABLE_SYNC = "NT_SYNC";

// Pre-defined (special) data packets
const uint8_t arpirobot::NET_TABLE_START_SYNC_DATA[] = {255, 255, '\n'};
const std::string arpirobot::NET_TABLE_END_SYNC_DATA = "\377\377\377\n";

////////////////////////////////////////////////////////////////////////////////
/// NetworkManager
////////////////////////////////////////////////////////////////////////////////

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
std::unordered_map<std::string, std::string> NetworkManager::ntSyncData;

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

bool NetworkManager::sendNtRaw(const_buffer buffer){
    if(isDsConnected){
        try{
            boost::asio::write(netTableClient, buffer);
            return true;
        }catch(const boost::system::system_error &err){
            if(err.code() == boost::asio::error::eof || err.code() == boost::asio::error::connection_reset){
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
    return sendNtRaw(boost::asio::buffer(data));
}

void NetworkManager::sendLogMessage(std::string message){
    if(isDsConnected){
        try{
            boost::asio::write(logClient, boost::asio::buffer(message));
        }catch(const boost::system::system_error &err){
            if(err.code() == boost::asio::error::eof || err.code() == boost::asio::error::connection_reset){
                handleDisconnect(logClient);
            }
        }
    }
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
            isDsConnected = true;
            Logger::logInfo("Drive station connected.");
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

        if(NetworkTableInternal::isInSync()){
            NetworkTableInternal::abortSync();
            Logger::logDebug("Net table sync aborted due to drive station disconnect.");
        }

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
                handleNetTableData();
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
            Logger::logDebug("Starting net table sync.");
            ntSyncData.clear();
            NetworkTableInternal::startSync();
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
            NetworkTableInternal::finishSync(ntSyncData);
        }else if (delim != std::string::npos){
            std::string key = std::string(subset.begin(), subset.begin() + delim);
            std::string value = std::string(subset.begin() + delim + 1, subset.begin() + subset.length() - 1);
            if(NetworkTableInternal::isInSync()){
                ntSyncData[key] = value;
            }else{
                NetworkTableInternal::setFromDs(key, value);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// NetworkTable
////////////////////////////////////////////////////////////////////////////////

void NetworkTable::set(std::string key, std::string value){
    NetworkTableInternal::setFromRobot(key, value);
}

std::string NetworkTable::get(std::string key){
    return NetworkTableInternal::get(key);
}

bool NetworkTable::has(std::string key){
    return NetworkTableInternal::has(key);
}

////////////////////////////////////////////////////////////////////////////////
/// NetworkTableInternal
////////////////////////////////////////////////////////////////////////////////

std::unordered_map<std::string, std::string> NetworkTableInternal::data;
std::mutex NetworkTableInternal::lock;
bool NetworkTableInternal::inSync = false;

bool NetworkTableInternal::isInSync(){
    return inSync;
}

void NetworkTableInternal::startSync(){
    lock.lock();
    inSync = true;
    Logger::logDebug("Starting sync from robot to DS.");

    if(!NetworkManager::sendNtRaw(boost::asio::buffer(NET_TABLE_START_SYNC_DATA, 3))){
        // abortSync will be called by handleDisconnect and lock will be released
        return;
    }

    sendAllValues();
    if(!inSync){
        return;
    }

    Logger::logDebug("Ending sync from robot to DS. Waiting for DS to sync data to robot.");
    NetworkManager::sendNtRaw(boost::asio::buffer(NET_TABLE_END_SYNC_DATA, 4));
}

void NetworkTableInternal::sendAllValues(){
    for(const auto &it : data){
        if(!inSync){
            return;
        }
        NetworkManager::sendNt(it.first, it.second);
    }
}

void NetworkTableInternal::finishSync(std::unordered_map<std::string, std::string> dataFromDs){
    Logger::logDebug("Got all sync data from DS to robot.");

    for(const auto &it : dataFromDs){
        data[it.first] = it.second;
    }

    inSync = false;
    lock.unlock();
    Logger::logInfo("Network table sync complete.");
}

void NetworkTableInternal::abortSync(){
    inSync = false;
    lock.unlock();
    Logger::logWarning("Network table sync aborted.");
}

void NetworkTableInternal::setFromRobot(std::string key, std::string value){
    // Do not allow \n or 255 in the key or value
    std::replace(key.begin(), key.end(), (char)255, '\0');
    std::replace(key.begin(), key.end(), '\n', '\0');
    {
        std::lock_guard<std::mutex> l(lock);
        data[key] = value;
    }
    NetworkManager::sendNt(key, value);
}

void NetworkTableInternal::setFromDs(std::string key, std::string value){
    std::lock_guard<std::mutex> l(lock);
    data[key] = value;
}

std::string NetworkTableInternal::get(std::string key){
    std::lock_guard<std::mutex> l(lock);
    auto it = data.find(key);
    if(it != data.end()){
        return data[key];
    }
    return "";
}

bool NetworkTableInternal::has(std::string key){
    std::lock_guard<std::mutex> l(lock);
    auto it = data.find(key);
    return it != data.end();
}
