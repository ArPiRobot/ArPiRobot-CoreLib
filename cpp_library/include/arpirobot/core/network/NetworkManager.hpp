#pragma once

#include <asio.hpp>
#include <functional>
#include <array>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>

#include <arpirobot/core/network/MainVmon.hpp>
#include <arpirobot/core/network/ControllerData.hpp>
#include <arpirobot/core/network/NetworkTable.hpp>

using namespace asio::ip;
using namespace asio;

namespace arpirobot{

    /*
    *
    * Networking protocol
    * The drive station uses four ports to communicate with the robot.
    * Controller Port  (UDP 8090):
    *     Data is only received from the DS. Data is not sent from the robot to the DS on this port.
    *     The controller port is a UDP port on port 8090. It is used to send controller data in the following packet format
    *     [controllerNumber,axisCount,buttonCount,dpadCount,axis1_h,axis1_l,...,buttons1,buttons2,...,dpads1,dpads2,...,\n]
    *     
    *     - The controller number and counts are 8 bit integers (unsigned)
    *     - axes are sent as signed 16-bit integers (high byte, low byte)
    *     - buttons are 1 (pressed) or 0 (not pressed) Each button byte encodes the state of eight buttons.
    *           The most significant bit is the smallest numbered button. For example in the first 
    *           button byte sent (buttons1) the most significant bit is button #0's state. The MSB of the
    *           second byte (buttons2) is the 9th button's state.
    *     - the dpad is 0 (no direction) or 1-8 (where 1 is up rotate clockwise)
    *                     8  1  2
    *                     7  0  3
    *                     6  5  4
    *           Two dpads are sent per byte. As with the buttons the most significant 4 bits represent 
    *           the lowest numbered dpad.
    * Command port (TCP 8091):
    *     Data is only received from the DS. Data is not sent from the robot to the DS on this port. Commands end with a newline.
    *     Commands are sent to the robot on this port as ASCII strings
    *     Some commands include:
    *     "ENABLE" = Enable the robot
    *     "DISABLE" = Disable the robot
    *     "NT_SYNC" = Start network table sync (always triggered by drive station)
    * Net Table port (TCP 8092):
    *     Data is sent and received on the net table port.
    *     New keys are sent to the drive station in the format shown below
    *     "[KEY]",255,"[VALUE]"
    *     Key and value are ASCII text. 255 is an unsigned int (this is used because it is not a valid ASCII character)
    *     Data received from the DS is in the same format and will cause the robot's net table to 
    *     set the given key to the given value.
    *     During a sync (sync must be started from the drive station by sending a command to the robot) 
    *     the robot sends the net table sync start sequence then sends all key/value pairs followed by the
    *     net table sync stop sequence. The robot then waits for the drive station to send any key/value pairs
    *     that the robot is missing. The drive station then sends the net table sync stop sequence and the robot
    *     then "ends" the sync.
    * Log port  (TCP 8093):
    *     Log messages are sent as strings from the robot to the drive station on this port. No data is sent to the robot
    *     from the drive station on this port.
    * 
    */


    // Commands from drive station
    const extern std::string COMMAND_ENABLE;
    const extern std::string COMMAND_DISABLE;
    const extern std::string COMMAND_NET_TABLE_SYNC;

    // Pre-defined (special) data packets
    const extern uint8_t NET_TABLE_START_SYNC_DATA[];
    const extern std::string NET_TABLE_END_SYNC_DATA;

    /**
     * \class NetworkManager network.hpp arpirobot/core/network.hpp
     * 
     * Used by BaseRobot to manage networking (with drive station)
     * Should not be used directly from user code
     */
    class NetworkManager{
    public:

        /**
         * Start networking protocol
         * @param enableFunc The function to call when the enable command is received
         * @param disableFunc The function to call when the disable function is received
         */
        static void startNetworking(std::function<void()> enableFunc, std::function<void()> disableFunc);

        /**
         * Stop networking
         */
        static void stopNetworking();

    private:

        /**
         * Send raw net table data
         * @param buffer Raw data to send to network table client
         */
        static bool sendNtRaw(const_buffer buffer);

        /**
         * Send a network table key/value pair to the network table client
         * @param key The key for the pair
         * @param value The value for the pair
         */
        static bool sendNt(std::string key, std::string value);

        /**
         * Send a message to the log client
         * @param message The message to send
         */
        static void sendLogMessage(std::string message);

        /**
         * Set a device as the main vmon
         * @param vmon A pointer to the device that is the main vmon.
         */
        static void setMainVmon(MainVmon *vmon);

        /**
         * Check if the given device is the main voltage monitor
         * @param vmon A pointer to the device that is the main vmon
         * @return true if the given device is the main vmon, else false
         */
        static bool isMainVmon(MainVmon *vmon);

        /**
         * Send the given voltage to the net table client using correct key
         * @param voltage The voltage to send
         */
        static void sendMainBatteryVoltage(double voltage);

        /**
         * Internal function to run the asio loop
         */
        static void runNetworking();

        /**
         * Handle data sent from client (async receive)
         * @param client The client to receive data from
         */
        static void receiveFrom(const tcp::socket &client);

        /**
         * Called when DS connects or disconnects
         */
        static void handleConnectionStatusChanged();

        // Boost async handler functions

        static void handleAccept(const tcp::socket &client, const std::error_code &ec);
        static void handleDisconnect(const tcp::socket &client);
        static void handleTcpReceive(const tcp::socket &client, const std::error_code &ec, std::size_t count);
        static void handleUdpReceive(const std::error_code &ec, std::size_t count);

        static void handleCommand();

        static void handleNetTableData();

        static void handleControllerData(std::vector<uint8_t> &data);

        static std::unordered_map<int, std::shared_ptr<ControllerData>> controllerData;

        // Thread for network io service
        static std::thread *networkThread;

        // Status
        static bool isDsConnected;
        static bool networkingStarted;

        // TODO: Main vmon

        // Read buffers (only receive data from command, controller, net table ports)
        static std::array<uint8_t, 32> tmpCommandRxBuf;
        static std::array<uint8_t, 32> tmpNetTableRxBuf;
        static std::array<uint8_t, 32> tmpControllerRxBuf;

        // These hold data received from multiple packets (above buffers are one packet use)
        static std::vector<uint8_t> commandRxData;
        static std::vector<uint8_t> netTableRxData;

        // Boost ASIO stuff
        static io_service io;
        static io_service::work wk; // Keep run from exiting

        static udp::socket controllerSocket;
        static tcp::acceptor commandSocketAcceptor;
        static tcp::acceptor netTableSocketAcceptor;
        static tcp::acceptor logSocketAcceptor;

        // Connected clients (drive station)
        static udp::endpoint controllerDataEndpoint;
        static tcp::socket commandClient;
        static tcp::socket netTableClient;
        static tcp::socket logClient;

        // Callback for enable and disable events (this are private functions in BaseRobot)
        // Doing this way makes it hard for other code to call enable / disable for the robot
        static std::function<void()> enableFunc;
        static std::function<void()> disableFunc;

        static std::unordered_map<std::string, std::string> ntSyncData;

        static MainVmon *mainVmon;


        friend class MainVmon;
        friend class Logger;
        friend class NetworkTable;
        friend class Gamepad;
    };
}