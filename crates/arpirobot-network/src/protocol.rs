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
//!
//! # Handshaking
//!
//! Upon first connecting, the client initiates a handshake with the server. The client's handshake
//! is used to indicate which protocol versions the client supports. The server must choose a
//! protocol it also supports from the client's list. If there are no protocol versions in common,
//! the server chooses [`PROTOCOL_INVALID`]. The server then responds with the chosen version.
//!
//! Handshaking **must** be possible between any version pairing of client / server as it is the
//! handshake that is used to determine if the client and server are compatible. To achieve this,
//! the handshaking sequence and values used during handshaking **must never** be changed. Because
//! of this, handshaking does not use general messages (or message framing, etc) described below.
//! Handshaking is a special case process implemented independently.
//!
//! A handshake message is a sequence of unsigned 32-bit integers (little endian), terminated with 
//! [`PROTOCOL_INVALID`]. The sequence before [`PROTOCOL_INVALID`] is received are the list of versions
//! supported (if sent by the client) or selected (if sent by the server).
//!
//! ```txt
//! | VERSION_A | VERSION_B | ... | PROTOCOL_INVALID |
//! ```
//!
//! It **is** valid to have a handshake message that contains no versions before [`PROTOCOL_INVALID`].
//! Such a message indicates that no versions are supported (client) or selected (server).
//! 
//! Handshaking occurs only on the command port.
//!
//!
//! # Message Framing
//!
//! Message data is split into different "frames" for each message using a delimiter byte. Frames
//! and messages map 1 to 1. That is to say that each frame contains exactly one message and it
//! contains that message in full.
//!
//! Frames include a delimiter byte at **both** the start and end of each message. At the end of the
//! message, a delimiter byte informs the receiver that it has the entire message. At the start of
//! the message, a delimiter serves only to "force" the receiver to clear previously received data
//! (eg if garbage were sent or a previous message was corrupted) to ensure the next message is not
//! missed (unless the next message is itself corrupted too)
//!
//! ```txt
//! | MESSAGE_FRAME (~)                         |
//! | DELIM (1) | FRAME_CONTENT (~) | DELIM (1) |
//! ```
//! 
//! Between the delimiters is the content of the message frame. The frame content is encoded using
//! COBS to ensure that the delimiter byte never appears within the frame content.
//!
//! The frame content includes two parts (both encoded). First is the message being sent. 
//! The second is a CCITT-FALSE CRC (16-bits, little endian) of the message (pre-encode) appended.
//!
//! ```txt
//! | MESSAGE_FRAME (~)                                          |
//! | DELIM (1) | FRAME_CONTENT (~)                   | DELIM(1) |
//! | DELIM (1) | ENCODED_MESSAGE(~) | ENCODED_CRC(~) | DELIM(1) |
//!                      ^                    ^
//!                      | cobs               | cobs
//!                      v                    v
//!                   MESSAGE(~) ----------> CRC(2)
//!                              ccitt-false
//! ```
//!
//! While the framing structure imposes no strict limit on frame length, the frame content is
//! limited in practice due to messages being limited to 4096 bytes (note that the frame content
//! could be longer as there are two additional CRC bytes -> 4098 bytes before COBS encoding, which
//! could be at most 4115 bytes after encoding). Plus two additional delimiter bytes
//! 
//! This length limitation exists for the following reasons:
//!
//! - It is not expected to currently have any messages (including log messages) that exceed 4KiB
//! - Limiting message sizes allows a more effecient frame encoder/decoder implementation
//! - The 16-bit CRC becomes less useful eventually (though closer to 16KiB) anyway
//!
//!
//!
//! # Message Data
//!
//! Each message will be sent in-full in a single frame. The structure of the message data itself
//! depends on the port. Each port has a set of message data structures which will be serialized 
//! into raw bytes to be sent as a single message.
//!
//! TODO: Finish this section


use cobs::{CobsDecoderOwned, CobsEncoder};



// =================================================================================================
// # Handshaking
// =================================================================================================

/// Used to indicate there is no valid protocol version during handshaking
///
/// THIS MUST NEVER CHANGE OR IT WILL BREAK HANDSHAKING!
const PROTOCOL_INVALID: u32 = 0;

/// The current protocol version implemented by this side
///
/// This must be changed when any of the following change:
///
/// - Constants, enums, structs, and format used to frame messages
/// - Constants, enums, structs serialized as messages
/// - Serializer / deserializer library version or settings if the format is not backwards
///   compatible in how it serializes / encodes data
/// - Bugs are fixed that change frame structure or crc calculation generally
///
///
/// The protocol version does not need to change when:
///
/// - The COBS library version changes (COBS is standardized)
/// - CRC implementation changes (but algorithm does not)
/// - Bugs are fixed that correct frame structure or crc calculation in specific cases
const PROTOCOL_VERSION: u32 = 1;


/// Parser to parse handshake message data into a set of versions
pub struct HandshakeParser {
    /// Holds bytes of each 32-bit integer during decoding
    buf: [u8; 4],

    /// Tracks how many bytes are in `buf`
    count: usize,

    /// Holds versions sent so far
    versions: Vec<u32>
}

impl HandshakeParser {
    pub fn new() -> Self {
         return Self {
            buf: [0u8; 4],
            count: 0,

            // It is not expected that client / server will ever support multiple protocol versions
            // So only allocate space for one version in this vector. But if that were to change in
            // the future this could grow on demand
            versions: Vec::with_capacity(1),
         };
    }

    /// Parses a byte as part of a handshake
    /// Parameters:
    ///     b: The received byte
    /// Returns: 
    ///     A vector of supported versions when the handshake message is fully received
    ///     None until a full handshake message has been parsed
    ///     NOTE: Once this returns a vector, the caller must call reset() before reuse
    pub fn parse(&mut self, b: &u8) -> Option<&Vec<u32>> {
        self.buf[self.count] = *b;
        self.count += 1;
        if self.count == 4 {
            let v = u32::from_le_bytes(self.buf);
            if v == 0 {
                return Some(&self.versions); 
            } else {
            
            }
            self.count = 0;
        }
        return None;
    }
    
    /// Reset the parser to parse a fresh handshake
    pub fn reset(&mut self) {
        self.count = 0;
        self.versions.clear();
    }
}


/// Possible errors while creating a handshake message
pub enum HandshakeCreateError {
    /// The buffer provided to write the message into was too small
    DestBufTooSmallError,
}

/// Handshake message / response generator
pub struct HandshakeGenerator { }

impl HandshakeGenerator {
    /// Gets the size of a handshake message (in bytes) that lists `n` versions
    pub const fn size_for_num_versions(n: usize) -> usize {
        return size_of::<u32>() * (n + 1);
    }

    /// Selects a protocol version that this local side supports from the set in `remote_versions`.
    /// A handshake message will be generated into `outbuf` indicating the selected version.
    /// `outbuf` must be large enough to indicate one version (see 
    /// [`size_for_num_versions`](HandshakeGenerator::size_for_num_versions)).
    ///
    /// If no supported version was indicated in `remote_versions`, this will instead generate a
    /// response message indicating that no version was selected. The size returned from this 
    /// function can be checked against values from 
    /// [`size_for_num_versions`](HandshakeGenerator::size_for_num_versions) to determine if a 
    /// version was selected or not.
    ///
    /// If `outbuf` is too small, this function will return
    /// [`DestBufTooSmallError`](HandshakeCreateError::DestBufTooSmallError) instead of the 
    /// generated message size
    pub fn for_remote_versions(outbuf: &mut [u8], remote_versions: &Vec<u32>) -> Result<usize, HandshakeCreateError> {
        if remote_versions.contains(&PROTOCOL_VERSION) {
            // Remote side is compatible. Respond with this side's version
            return HandshakeGenerator::create(outbuf, &vec![PROTOCOL_VERSION]);
        } else {
            // No compatible version, respond invalid
            return HandshakeGenerator::create(outbuf, &vec![]);
        }
    }
    
    /// Creates a handshake message indicating this side's protocol version. The message will be
    /// generated into `outbuf` (must be large enough to hold one version, see
    /// [`size_for_num_versions`](HandshakeGenerator::size_for_num_versions)
    ///
    /// This function will return the size of the generated message unless `outbuf` is too small, in
    /// which case it will return
    /// [`DestBufTooSmallError`](HandshakeCreateError::DestBufTooSmallError)
    pub fn for_local_version(outbuf: &mut [u8]) -> Result<usize, HandshakeCreateError> {
       return HandshakeGenerator::create(outbuf, &vec![PROTOCOL_VERSION]);
    }

    /// Creates a handshake message indicating the supported `versions` provided. The message will
    /// be generated into `outbuf`.
    ///
    /// This function returns the size of the generated message, unless `outbuf` is too small for
    /// the number of `versions` provided. See
    /// [`size_for_num_versions`](HandshakeGenerator::size_for_num_versions)
    ///
    /// THE STRUCTURE OF THESE MESSAGES MUST NEVER CHANGE
    fn create(outbuf: &mut [u8], versions: &Vec<u32>) -> Result<usize, HandshakeCreateError> {
        if outbuf.len() < HandshakeGenerator::size_for_num_versions(versions.len()) {
            return Err(HandshakeCreateError::DestBufTooSmallError);
        }
        let mut len: usize = 0;
        for i in 0..versions.len()+1 {
            let version = if i < versions.len() { versions[i] } else { PROTOCOL_INVALID };
            let dest = &mut outbuf[(i*4)..(i*4)+4];
            dest.copy_from_slice(&version.to_le_bytes());
            len += 4;
        }
        return Ok(len);
    }
}




// =================================================================================================
// Message Framing
// =================================================================================================

/// Message frame delimiter byte
/// If this is changed, `PROTOCOL_VERSION` must be incremented
const FRAME_DELIM: u8 = 0x00;

/// Maximum message (**not** frame or frame content) length
/// If this is changed, `PROTOCOL_VERSION` should be incremented
const MAX_MSG_LEN: usize = 4096;

/// Number of CRC bytes
const CRC_LEN: usize = 2;

/// Number of delimiter bytes in a frame
const FRAME_DELIM_COUNT: usize = 2;

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

/// Parse data received as a possible message frame and decode the messages
/// Parameters:
///     dec: The COBS decoder instance being used by this parser
///     b:   The byte
/// Returns:
///     true if a complete and valid (passing CRC check) message is available in the decoder's
///     buffer.
///     false otherwise
fn parse_frame_byte(dec: &mut CobsDecoderOwned, b: u8) -> bool {
    // xor would be required if FRAME_DELIM were not 0x00. Implement to support that.
    // The compiler will optimize this away if FRAME_DELIM is a constant 0x00
    match dec.feed(b ^ FRAME_DELIM) {
        Ok(None) => return false, // Not a complete message
        Err(_) => return false, // Ignore decode errors, assume corrupted or too long message
        Ok(Some(n)) => {
            // Got a frame of size n bytes
            if n < CRC_LEN {
                return false; // Not enough bytes for a CRC
            }
            let data = dec.dest();
            let crc_recv: u16 = u16::from_le_bytes([data[n-2], data[n-1]]);
            let crc_calc = ccitt_false(&data[0..n-2]);
            return crc_calc == crc_recv;
        },
    }
}

/// Possible errors during frame creation
enum FrameCreateError {
    MessageTooLongError,
    DestBufTooSmallError,
}

/// Get the largest possible **frame** size for a given **message** size
/// The size can vary due to COBS encoding
/// Parameters:
///     msg_len: The length of the message
/// Returns:
///     The max possible frame lenth after CRC, delimiters, and COBS encoding
///     In other words, the size `outframe` needs to be when passed to `create_frame`
fn calc_max_frame_len(msg_len: usize) -> usize {
    return FRAME_DELIM_COUNT + cobs::max_encoding_length(msg_len + CRC_LEN);
}

/// Create a frame for a given message
/// Parameters:
///     inmsg: The raw message payload data
///     outframe: Buffer to store the frame in
/// Returns:
///     Ok on success
///     Err on error (eg if frame output buffer is too small)
pub fn create_frame(outframe: &mut[u8], inmsg: &[u8]) -> Result<usize, FrameCreateError> {
    // Enforce message length limit
    if inmsg.len() > MAX_MSG_LEN {
        return Err(FrameCreateError::MessageTooLongError)
    }
    
    // Calculate the CRC **before** encoding the message data
    let crc = ccitt_false(inmsg);

    // Constructing an entire frame, so start with a delimiter
    outframe[0] = FRAME_DELIM;

    // Encode message plus CRC into outframe buffer starting after the delimiter
    let outcontent = &mut outframe[1..];
    let mut enc = CobsEncoder::new(outcontent);
    let Ok(_) = enc.push(inmsg) else {
        return Err(FrameCreateError::DestBufTooSmallError) 
    };
    let Ok(_) = enc.push(&crc.to_le_bytes()) else {
        return Err(FrameCreateError::DestBufTooSmallError);
    };
    let frame_content_len = enc.finalize();

    // If a delimiter other than 0x00 is used, need to xor each byte of the encoded data
    // with the delimiter to shift the encoding. This is not currently used, but implemented to
    // support if desired in the future. The compiler will optimize this away when not needed.
    if FRAME_DELIM != 0x00 {
        for i in 0..frame_content_len {
            outcontent[1 + i] ^= FRAME_DELIM;
        }
    }

    // Full frame length with the two delimiters
    let frame_len = frame_content_len + FRAME_DELIM_COUNT;

    // Insert delimiter onto end of frame
    if frame_len <= outframe.len(){
        outframe[frame_len - 1] = FRAME_DELIM;
        return Ok(frame_len);
    } else {
        return Err(FrameCreateError::DestBufTooSmallError);
    }
}



// =================================================================================================
// Command Port Messages
// =================================================================================================

// TODO: Make sure this can be serialized efficiently
/// Commands that can be sent from the drive station to the robot
pub enum Command {
    Disable,
    Enable,
    NetTableSync
}

pub struct CommandParser {
    dec: CobsDecoderOwned
}

impl CommandParser {
    pub fn new() -> Self {
        return Self {
            dec: CobsDecoderOwned::new(MAX_MSG_LEN + 2), // 2 byte CRC
        };
    }
    pub fn parse(&mut self, b: u8) -> Option<Command> {
        if parse_frame_byte(&mut self.dec, b) {
            // The decoder contains a decoded message (w/ valid crc)
            let msg = &self.dec.dest()[1..];
            // TODO: Deserialize the message into a Command
            self.dec.reset();
            // TODO: Return the command
            return None;
        } else {
            // Not a complete message yet
            return None;
        }
    }
}

pub struct ComandGenerator {
    // TODO: pub fn create_message(cmd: Command)
}











/*



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

*/
