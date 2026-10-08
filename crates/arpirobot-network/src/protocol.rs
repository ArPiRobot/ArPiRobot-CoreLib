//! This module defines data structures used for ArPiRobot network messages
//! The ArPiRobot network protocol uses three different ports
//! 
//! - Command Port (TCP): Used to send commands from the drive station to the robot. This port is
//!   also used for initial handshaking between the drive station and robot.
//! 
//! - Network table Port (TCP): Used to synchronize key / value pairs between the drive station and
//!   the robot. This is implemented on a separate TCP port from the command port to ensure that
//!   important commands (eg disabling the robot) are not queued behind network table traffic.
//! 
//! - Controller Port (UDP): Used by the drive station to send gamepad controller data to the robot.
//!   Implemented using UDP since minimizing latency is more important than dropped packets.
//!   Additionally, when many packets are being dropped, it is much more important that the latest
//!   controller data (which is re-sent at a fast pace anyway) gets received over enforcing ordering



/// General Constants

/// Used to indicate there is no valid protocol version during handshaking
pub const PROTOCOL_INVALID: u32 = 0;

/// The current protocol version implemented by this side
/// This must be changed every time a message data structure or message id is changed
/// This includes enums since serde will serialize them differently when they change
pub const PROTOCOL_VERSION: u32 = 1;



/// Command Port Messages

/// Command ids used with the CommandMessage command field
pub enum Command {
    Enable,
    Disable,
    NetTableSync,
}

/// Handshake message
/// This message MUST NOT be modified to avoid breaking handshaking with older versions
/// Upon connecting, the client sends the handshake message to the server. The client's message can
/// contain more than one protocol version if supported. The server will then send a single protocol
/// version in the response. This version will be whichever compatible version the server selects or
/// PROTOCOL_INVALID if there is no compatible version supported by the server
pub struct HandshakeMessage {
    pub protocol_versions: Box<[u32]>,
}

/// Command message
/// Used by the client (drive station) to send a message to the robot (server)
pub struct CommandMessage {
    /// Command ID
    pub command: Command,
}



/// Network Table Messages

/// All the types a network table value could take
pub enum NetTableValue {
    Integer(Box<[i64]>),
    Float(Box<[f64]>),
    String(Box<[str]>),
}

/// Network table raw key/value pair message
/// Either client or server can send such a message to synchronize the pairs
pub struct NetTableMessage {
    /// Net table entry key
    pub key: str,

    /// Net table entry value
    pub value: NetTableValue,
}
