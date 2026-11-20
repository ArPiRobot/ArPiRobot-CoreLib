//! This module defines data structures, parsers, and emitters used for ArPiRobot network messages
//!
//!
//!
//! # Ports
//!
//! The ArPiRobot network protocol uses four different ports
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
//!
//! - Log Port (TCP): Used by the robot (server) to send log messages to the drive station (client)
//!
//!
//!
//! # Message Framing
//!
//! Raw message data is split into different "frames" for each message using a delimiter byte. The
//! delimiter byte should be sent at BOTH the start and end of each message. At the end of the
//! message, a delimiter byte informs the receiver that it has the entire message. At the start of
//! the message, a delimiter serves only to "force" the receiver to clear previously received data
//! (eg if garbage were sent or a previous message was corrupted) to ensure the next message is not
//! missed (unless the next message is itself corrupted too)
//!
//! ```txt
//! | DELIM (1) | CONTENT (~)          | DELIM (1) |
//! ```
//! 
//! Between the delimiters is the content of the message. The content of the message is encoded
//! using COBS to ensure that the delimiter byte never appears within the message content.
//!
//! The message content includes two parts. First is the raw message payload (the data actually
//! being sent in the message). The second is a 16-bit CRC (CCITT-FALSE). The CRC is of the message
//! payload data and used by the receiver to verify the message integrity. Verification of message
//! integrity is **not** optional. Receivers are required to verify integrity before considering a
//! frame a valid message. The CRC value is sent little endian.
//!
//! ```txt
//! | DELIM (1) | PAYLOAD (~) | CRC(2) | DELIM (1) |
//! ```
//!
//! While the framing structure imposes no strict limit on payload length, the payload is limited to
//! 4096 bytes. There are a few reasons for this decision
//!
//! - It is not expected to currently have any messages (including log messages) that exceed this
//!   length
//! - Limiting message sizes allows a more effecient frame encoder/decoder implementation
//! - The 16-bit CRC becomes less useful eventually anyway (closer to 16KiB than 4KiB)
//!
//!
//!
//! # TODO: Other sections


use cobs::{CobsDecoderOwned};
use log;


// =================================================================================================
// # General Constants
// =================================================================================================

/// Used to indicate there is no valid protocol version during handshaking
const PROTOCOL_INVALID: u32 = 0;

/// The current protocol version implemented by this side
/// This must be changed when any of the following change:
///     - Message IDs, constants, enum values
///     - Message data structures being serialized / deserialized
///     - Serializer / deserializer library version or format settings
///     - A different type of CRC is used in message framing
/// The protocol version does not need to change when:
///     - The COBS library version changes (COBS is standardized)
const PROTOCOL_VERSION: u32 = 1;



// =================================================================================================
// Message Framing
// =================================================================================================

/// Message frame delimiter byte
const FRAME_DELIM: u8 = 0x00;

/// Maximum message **frame** size (payload plus CRC)
const FRAME_MAX_LEN: usize = 4096 + 2;

/// Parses message frames
pub trait FrameParser {
    /// Calculate the CCITT-FALSE CRC of the given data
    /// Parameters:
    ///     data: Data slice
    /// Return:
    ///     Calculated CRC of the given data
    fn ccitt_false(data: &[u8]) -> u16 {
        let mut crc = 0xFFFF;
        for b in data {
            for i in 0..8 {
                let bit: u8 = ((b >> (7 - i) & 1) == 1) as u8;
                let c15 = ((crc >> 15 & 1) == 1) as u8;
                if c15 ^ bit != 0 {
                    crc ^= 0x1021;
                }
            }
        }
        return crc;
    }

    /// Feed a byte into the frame parser
    /// Parmeters:
    ///     dec: The COBS decoder instance being used by this parser
    ///     b:   The byte
    /// Returns:
    ///     true if a complete and valid (passing CRC check) message is available in the decoder's
    ///     buffer.
    ///     false otherwise
    fn feed(&self, dec: &mut CobsDecoderOwned, b: u8) -> bool {
        // xor would be required if FRAME_DELIM were not 0x00. Implement to support that.
        // The compiler will optimize this away if FRAME_DELIM is a constat 0x00
        match dec.feed(b ^ FRAME_DELIM) {
            Ok(None) => return false, // Not a complete message
            Err(_) => return false, // Ignore decode errors, assume corrupted message
            Ok(Some(n)) => {
                // Got a frame of size n bytes
                if n < 2 {
                    return false; // Not enough bytes for a CRC
                }
                let data = dec.dest();
                let crc_recv: u16 = u16::from_le_bytes([data[n-2], data[n-1]]);
                let crc_calc = Self::ccitt_false(&data[0..n-2]);
                return crc_calc == crc_recv;
            },
        }
    }
}

/// TODO: cobs encoder

impl MessageParser{
    pub fn new() -> Self{
        return Self {
            started: false,
            escaped: false,
            buffer: [0u8; MSG_MAX_LEN],
        };
    }

    /// Handle a byte b. Returns true if a complete and valid message is available
    pub fn parse(&mut self, b: u8) -> bool{
        if self.escaped {
            if b == 
        }
        return false;
    }
}


/// Maximum message payload length (including CRC)


/// Message payloads (serialized / deserialized to bytes) 

/// Command ids used with the CommandMessage command field
pub struct Command { }
impl Command {
    pub const DISABLE:  u8   = 1;
    pub const ENABLE:   u8   = 2;
    pub const NT_SYNC:  u8   = 3;
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
    String(Box<[String]>),
}

/// Network table raw key/value pair message
/// Either client or server can send such a message to synchronize the pairs
pub struct NetTableMessage {
    /// Net table entry key
    pub key: String,

    /// Net table entry value
    pub value: NetTableValue,
}



/// Log Messages

/// Log message being sent from server to client
pub struct LogMessage {
    /// Message level
    pub level: log::Level,

    /// Message contents
    pub message: str,
}
