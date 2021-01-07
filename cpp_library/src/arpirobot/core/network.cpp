#include <arpirobot/core/network.hpp>
#include <arpirobot/core/log.hpp>

using namespace arpirobot;


// Commands from drive station
const extern char *COMMAND_ENABLE = "ENABLE";
const extern char *COMMAND_DISABLE = "DISABLE";

// Pre-defined (special) data packets
const extern uint8_t NET_TABLE_START_SYNC_DATA[] = {255, 255, '\n'};
const extern uint8_t NET_TABLE_END_SYNC_DATA[] = {255, 255, 255, '\n'};

// Static variables for NetworkManager
std::thread *NetworkManager::networkThread = nullptr;
bool NetworkManager::isDsConnected = false;
bool NetworkManager::networkingStarted = false;
std::array<uint8_t, 32> NetworkManager::commandRxBuffer;
std::array<uint8_t, 128> NetworkManager::netTableRxBuffer;
std::array<uint8_t, 128> NetworkManager::controllerRxBuffer;
io_service NetworkManager::io;
io_service::work NetworkManager::wk(NetworkManager::io);
udp::socket NetworkManager::controllerSocket(NetworkManager::io, udp::endpoint(udp::v4(), 8090));
tcp::acceptor NetworkManager::commandSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8091));
tcp::acceptor NetworkManager::netTableSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8092));
tcp::acceptor NetworkManager::logSocketAcceptor(NetworkManager::io, tcp::endpoint(tcp::v4(), 8093));
tcp::socket NetworkManager::commandClient(NetworkManager::io);
tcp::socket NetworkManager::netTableClient(NetworkManager::io);
tcp::socket NetworkManager::logClient(NetworkManager::io);


void NetworkManager::startNetworking(){
    if(!networkingStarted){
        if(networkThread != nullptr){
            networkThread->join();
            delete networkThread;
        }

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

void NetworkManager::runNetworking(){
    io.run();
}

void NetworkManager::handleAccept(const tcp::socket &client, const boost::system::error_code &ec){
    Logger::logDebugFrom("NetworkManager", client.remote_endpoint().address().to_string());
}