//! This module implements the server side communication via a single-threaded state machine.
//! The state machine uses non-blocking socket I/O (polling).
//! The caller is expected to manage the thread and ensure that no two threads call run() at the
//! same time.
//! The methods to queue data to be written **are** thread-safe to allow multiple callers on
//! different threads to queue data for write.
//!
//! Server State machine:
//!
//! ```txt
//!                          │
//!                          │START
//!                          │
//!                    ┌─────▼──────┐
//!                    │Disconnected│
//!                    │(No Clients)│◄───┐
//!                    └─────┬──────┘    │
//!                          │Client     │Client connects
//!                          │connect    │from different IP
//!                    ┌─────▼───────┐   │
//!                ┌──►│ Connecting  ├───┘
//! Client connects│   │(1-2 clients)│   ▲
//! from same IP   └───┴─────┬───────┘   │
//!                          │3 clients  │
//!                          │same IP    │
//!                    ┌─────▼─────┐     │Client disconnects
//!                ┌──►│ Connected ├─────┤(read error, read EOF, write error)
//!       Invalid  │   │(3 clients)│     │
//!       Handshake└───┴─────┬─────┘     │
//!                          │Valid      │
//!                          │Handshake  │
//!                    ┌─────▼─────┐     │
//!                    │ Handshook ├─────┘
//!                    │(3 clients)│
//!                    └───────────┘
//! ```
//!
//! Actions when transitioning into states:
//!
//! - Transition to Disconnected State:
//!     - Stop reading from clients
//!     - Stop listening on controller UDP port
//!     - Stop writing to clients
//!     - Disconnects any connected clients
//! - Transition to Connecting State:
//!     - Reset handshake buffer
//!     - Reset read buffers
//!     - Reset write buffers
//! - Transition to Connected State:
//!     - Start reading from the command client (only the one port)
//!     - Start writing to the command client (only the one port, for handshake responses)
//! - Transition to Handshook State:
//!     - Start reading from all clients
//!     - Start writing to all clients
//!     - Start listening on the controller UDP port
//!
//! Actions while in each state:
//! - While in Disconnected State:
//!     - Server accepts any incoming TCP connection
//! - While in Connecting State:
//!     - Server accepts incoming TCP connections from the same IP
//!     - Server rejects incoming TCP connections from different IPs
//! - While in Connected State:
//!     - Server parses any data received on the command port as part of a handshake until  
//!       a complete handshake has been received
//!     - Server responds to complete handshake data with a selected protocol version
//! - While in Handshook State:
//!     - Data received on each port (TCP or UDP) is parsed as a message of the type expected on  
//!       that port
//!     - Data queued to be written to each client will be written





use mio::net::{TcpListener, TcpStream};
use mio::{Events, Interest, Poll, Registry, Token, Waker};
use std::collections::VecDeque;
use std::net::{IpAddr, SocketAddr};
use std::io::{self, IoSlice, Write};
use std::io::Read;
use std::time::Duration;
use std::sync::{Mutex};
use log::{warn, error, info, debug};

use crate::protocol::{HandshakeParser, HandshakeGenerator};

/// Server states
#[derive(Clone, Copy)] 
enum State {
    /// No client is currently connected to any TCP port
    Disconnected, 
    
    /// A client is connected to at least one TCP port, but not all TCP ports
    Connecting,  
    
    /// The same client (by IP) is connected to all TCP ports, but handshake has not been completed
    Connected, 
    
    /// The same client (by IP) is connected to all TCP ports and handshake has been completed
    Handshook,
}

/// Tokens to identify what a polled event originates from
struct Tokens;
impl Tokens {
    // Server tokens
    pub const LISTENER_CTRL: Token = Token(0);
    pub const LISTENER_CMD: Token = Token(1);
    pub const LISTENER_NT:  Token = Token(2);
    pub const LISTENER_LOG: Token = Token(3);

    // Client tokens
    pub const STREAM_CMD: Token = Token(4);
    pub const STREAM_NT:  Token = Token(5);
    pub const STREAM_LOG: Token = Token(6);
    
    // Write buffer tokens
    pub const WBUF_CMD: Token = Token(7);
    pub const WBUF_NT:  Token = Token(8);
    pub const WBUF_LOG: Token = Token(9);
}

struct Clients {
    // TCP Sockets for each client
    pub cmd: Option<TcpStream>,
    pub nt: Option<TcpStream>,
    pub log: Option<TcpStream>,

    // IP address for connected clients (remote side)
    pub ip: Option<IpAddr>,
}

struct Parsers {
    handshake: HandshakeParser,
}

struct Listeners {
    // TODO: UDP listener
    cmd: TcpListener,
    nt: TcpListener,
    log: TcpListener,
}

/// Asynchronous write buffer (thread safe) for server. Buffer is FIFO
/// Used by multiple (server or non-server) threads to queue data to be written later by the server
/// thread (event loop) using an mio Waker
struct WBuffer {
    /// Data waiting to be written
    data: Mutex<VecDeque<u8>>,

    /// Used to wake server event loop to process the buffer
    waker: Waker,
}

impl WBuffer {
    /// Create a new async write buffer with the given `capacity`. The buffer's waker will be
    /// registered to the given mio `registry` using the provided `token` to signal when the buffer
    /// is not empty (meaning it can be read)
    pub fn new(capacity: usize, registry: &Registry, token: Token) -> io::Result<WBuffer> {
        let waker = Waker::new(registry, token)?;
        return Ok(Self {
            data: Mutex::new(VecDeque::with_capacity(capacity)),
            waker: waker
        });
    }
    
    /// Queue `data` into the buffer to be written later
    pub fn queue(&self, data: &[u8]) -> io::Result<()> {
        // Lock buffer
        let mut buf = self.data.lock().unwrap();

        // Use waker to indicate data is in a write buffer
        // Do this BEFORE putting data in the buffer since it may error
        // If this errors, queue is considered to have failed. In which case
        // we do not want data in the buffer either
        // Because this occurs while holding the buffer's mutex, this is safe
        // since a reader would need to take the mutex. So any reader woken by the Waker will
        // be able to read the expected data upon mutex release
        self.waker.wake()?;

        // Queue data to be written
        for b in data {
            buf.push_back(*b); 
        }

        return Ok(());
    }

    /// Flush data in the buffer to the given tcp `stream`. This will return any error from the tcp
    /// stream's write function. If Ok is returned, the write complted successfully.
    pub fn flush(&self, stream: &mut TcpStream) -> io::Result<usize> {
        // Lock buffer. Since socket I/O is non-blocking, will keep it locked during writes.
        let mut buf = self.data.lock().unwrap();
        
        // Write until done, would block, or an error occurs
        // Loop is needed because write syscall could be interrupted
        loop {    
            // Get internal buffer data as up to 2 contiguous slices
            let (slice1, slice2) = buf.as_slices();
            
            // write_vectored expects an array of IoSlices
            let io_slices = [IoSlice::new(slice1), IoSlice::new(slice2)];
            
            // Attempt to write
            match stream.write_vectored(&io_slices) {
                Ok(n) => {
                    // Wrote n bytes. Remove the first n elements from the buffer
                    buf.drain(..n);

                    // If the buffer is not empty, return would block for the caller to handle
                    // Expected that the caller will poll the stream to determine when it becomes
                    // writable again, then call this function again
                    if buf.is_empty() {
                        return Ok(n);
                    } else {
                        return Err(io::Error::new(io::ErrorKind::WouldBlock, "Write incomplete, would block."))
                    }
                },
                Err(e) => {
                    if e.kind() == io::ErrorKind::Interrupted {
                        // Syscall interrupted. Just retry the write
                    } else {
                        // Got an actual error
                        return Err(e);
                    }
                }
            }
        }
    }

    /// Clear the buffer
    pub fn reset(&self) {
        let mut buf = self.data.lock().unwrap();
        buf.clear();
    }
}

// Server implementation
pub struct Server {
    // Server state
    state: State,

    // Client connections
    client: Clients,
    
    // Listener instances
    listener: Listeners,

    // Polling infrastructure
    poll: Poll,
    events: Events,

    // Message parsers (include buffers) for reading from clients
    parsers: Parsers,

    // Write buffers (ring buffers) for writing to clients
    wbuffer_cmd: WBuffer,
    wbuffer_nt:  WBuffer,
    wbuffer_log: WBuffer,
}

impl Server {
    /// Create a new server instance
    /// Arguments:
    /// - ctrl_addr: Address to listen on for controller UDP datagrams
    /// - cmd_addr:  Address to listen on for command TCP connections
    /// - nt_addr:   Address to listen on for net table TCP connections
    /// - log_addr:  Address to listen on for log TCP connections
    pub fn bind(ctrl_addr: SocketAddr, cmd_addr: SocketAddr, nt_addr: SocketAddr, log_addr: SocketAddr) -> io::Result<Self> {
        let poll        = Poll::new()?;
        let wbuffer_cmd = WBuffer::new(64,   poll.registry(), Tokens::WBUF_CMD)?;
        let wbuffer_nt  = WBuffer::new(1042, poll.registry(), Tokens::WBUF_NT)?;
        let wbuffer_log = WBuffer::new(8192, poll.registry(), Tokens::WBUF_LOG)?;
        let mut s = Self {
            state: State::Disconnected,
            client: Clients {
                cmd: None,
                nt: None,
                log: None,
                ip: None,
            },
            listener: Listeners { 
                cmd: TcpListener::bind(cmd_addr)?,
                nt:  TcpListener::bind(nt_addr)?,
                log: TcpListener::bind(log_addr)?,
            },
            poll: poll,
            events: Events::with_capacity(16),
            parsers: Parsers {
                handshake: HandshakeParser::new(), 
            },
            wbuffer_cmd: wbuffer_cmd,
            wbuffer_nt:  wbuffer_nt,
            wbuffer_log: wbuffer_log,
        };
        s.poll.registry().register(&mut s.listener.cmd, Tokens::LISTENER_CMD, Interest::READABLE)?;
        s.poll.registry().register(&mut s.listener.nt,  Tokens::LISTENER_NT,  Interest::READABLE)?;
        s.poll.registry().register(&mut s.listener.log, Tokens::LISTENER_LOG, Interest::READABLE)?;
        return Ok(s);
    }
    
    /// Get the IP address of the current client
    /// If a client is not fully connected with a completed handshake, None is returned
    pub fn client_ip(&self) -> Option<IpAddr> {
        match self.state {
            State::Connected => return self.client.ip,
            _ => return None
        }
    }

    /// Run server event loop. This function **WILL** return. You must call it repeatedly.
    /// If an error is returned, the server may not function properly anymore
    /// The provided duration is the maximum amount of time to wait for events. The function may
    /// take a bit longer to return as it may handle events too. But the duration ensures that the
    /// function will return to allow proper thread shutdown when needed
    pub fn run(&mut self, duration: Option<Duration>) -> io::Result<()> {
        // Poll for events
        if let Err(e) = self.poll.poll(&mut self.events, duration) {
            match e.kind(){
                io::ErrorKind::WouldBlock |
                io::ErrorKind::Interrupted | 
                io::ErrorKind::TimedOut => {
                    return Ok(());
                },
                _ => return Err(e),
            }
        }

        // Take the events loop from self (so self can be mutable when invoking other functions)
        let events = std::mem::replace(&mut self.events, Events::with_capacity(0));
       
        // Process each event
        let res = self.handle_events(&events);

        // Restore events before returning
        self.events = events;
        return res;
    }




    // =============================================================================================
    // Event handling
    // =============================================================================================

    /// Process a set of polled events
    fn handle_events(&mut self, events: &Events) -> io::Result<()> {
        for event in events {
            let token = event.token();
            match token {
                Tokens::LISTENER_CMD | Tokens::LISTENER_NT | Tokens::LISTENER_LOG => {
                    self.handle_incoming_connection(token);
                },
                Tokens::LISTENER_CTRL => {
                    // TODO: Handle UDP data
                },
                Tokens::STREAM_CMD|Tokens::STREAM_NT|Tokens::STREAM_LOG => {
                    // TODO: Check readable vs writable. If writable, handle like WAKER_WRITE
                    self.handle_stream_readable(token);
                },
                Tokens::WBUF_CMD | Tokens::WBUF_NT | Tokens::WBUF_LOG => {
                    // TODO: Handle this by checking all the buffers and attempting writes
                    // TODO: Maybe split into three wakers so we don't have to do three locks?
                }
                _ => error!("Invalid token encountered"),
            }
        }
        return Ok(());
    }
   
    /// Handle readable events on TCP listeners (which indicates that a client is trying to
    /// connect)
    /// Arguments:
    /// - token: The token from Tokens struct identifying the server
    fn handle_incoming_connection(&mut self, token: Token) {
        loop {
            // Borrow inside the loop so borrow checker will know the last access per iteration
            // so it won't prevent calling self.do_ functions to transition states
            let (listener, stream) = match token {
                Tokens::LISTENER_CMD => (& self.listener.cmd, &mut self.client.cmd),
                Tokens::LISTENER_NT  => (& self.listener.nt,  &mut self.client.nt),
                Tokens::LISTENER_LOG => (& self.listener.log, &mut self.client.log),
                _ => {
                    error!("Invalid token encountered");
                    continue;
                }
            };
            match listener.accept() {
                Ok((s, a)) => {
                    match token {
                        Tokens::LISTENER_CMD => debug!("Accepted connection on command port"),
                        Tokens::LISTENER_NT  => debug!("Accepted connection on net table port"),
                        Tokens::LISTENER_LOG => debug!("Accepted connection on log port"),
                        _ => {}
                    }
                    match self.state {
                        State::Disconnected => {
                            // Accept first TCP connection to any port unconditionally
                            *stream = Some(s);
                            self.client.ip = Some(a.ip());
                            self.do_connecting();
                        },
                        State::Connecting => {
                            // Accept subsequent connections only if from the same ip as the first
                            if Some(a.ip()) == self.client.ip && stream.is_none() {
                                *stream = Some(s);
                                if self.client.cmd.is_some() && 
                                        self.client.nt.is_some() && 
                                        self.client.log.is_some() {
                                    self.do_connected();
                                }
                            } else {
                                // Got a mix of client ips. Drop all connections
                                // NOTE: This could maybe result in a denial of service attack
                                //       where a malicious client spams connection attempts in an
                                //       effort to prevent all three of a legitimate client's from
                                //       succeeding. Given current use cases, this is not considered
                                //       an issue in practice as robots will run their own networks
                                //       and it can be reasonably expected that malicious clients
                                //       will not exist on said network
                                warn!("Multiple clients attempted to connect. Disconnecting both");
                                drop(s);
                                self.do_disconnect();
                            }
                        }
                        _ => {
                            // New clients will not be accepted
                            warn!("A second client attempted to connect. Disconnecting it.");
                            drop(s);
                        }
                    }
                },
                Err(e) => {
                    match e.kind() {
                        io::ErrorKind::WouldBlock => break, // Have handled all incoming connections
                        io::ErrorKind::Interrupted => {},   // syscall interrupted. Retry
                        _ => warn!("Failed to accept incoming connection: {}", e),
                    }
                }
            }
        }
    }
    
    /// Handle a TCP stream which has bytes to read
    fn handle_stream_readable(&mut self, token: Token) {
        let mut chunk = [0u8; 1024];
        loop {
            // Borrow inside the loop so borrow checker can understand where the borrow ends each
            // iteration so the do_ functions can be called
            let (opt_stream, handler) : (&mut Option<TcpStream>, fn(&mut Self, &[u8])) = match token {
                Tokens::LISTENER_CMD => (&mut self.client.cmd, Self::handle_cmd_bytes),
                Tokens::LISTENER_NT  => (&mut self.client.nt,  Self::handle_nt_bytes),
                Tokens::LISTENER_LOG => (&mut self.client.log, Self::handle_log_bytes),
                _ => {
                    error!("Invalid token encountered");
                    continue;
                }
            };
            let Some(client) = opt_stream else {
                warn!("Tried to read command client, but it is not connected");
                return;
            };
            match client.read(&mut chunk) {
                Ok(0) => {
                    // Remote side closed cleanly (EOF)
                    info!("Client closed port. Disocnnecting.");
                    self.do_disconnect()
                },
                Ok(n) => {
                    match token {
                        Tokens::LISTENER_CMD => debug!("Read {} bytes on command port", n),
                        Tokens::LISTENER_NT  => debug!("Read {} bytes on NT port", n),
                        Tokens::LISTENER_LOG => debug!("Read {} bytes on log port", n),
                        _ => {}
                    }
                    handler(self, &chunk[0..n])
                },
                Err(e) => {
                    match e.kind() {
                        io::ErrorKind::WouldBlock => break,     // Have handled all data
                        io::ErrorKind::Interrupted => {},       // syscall interrupted. Retry
                        _ => { 
                            // Remote side dropped
                            info!("Client connection lost. Disconnecting.");
                            self.do_disconnect()
                        }
                    }
                }
            }
        }
    }

    // =============================================================================================
    // Reading from clients
    // =============================================================================================

    /// Handle a byte read from the command port during handshaking
    fn handle_cmd_byte_handshake(&mut self, b: &u8) {
        // The first thing send on the command port is handshake data. Until handshaking
        // completes, each byte is assumed to be part of the handshake.
        match self.parsers.handshake.parse(b) {
            Some(remote_versions) => {
                // Generate handshake response
                let mut response: [u8; 8] = [0u8; 8];
                debug!("Client versions: {:?}", remote_versions);
                let version = HandshakeGenerator::select_from_remote_versions(remote_versions);
                debug!("Server selected version: {}", version);
                let Ok(size) = HandshakeGenerator::for_single_version(&mut response, version) else {
                    error!("Failed to reply to client's handshake. Disconnecting");
                    self.do_disconnect();
                    return;
                };

                // Reset the handshake parser now. It would be legal for a client
                // to receive an invalid handshake (PROTOCOL_INVALID response)
                // And for that client to then attempt to handshake again. The server
                // will not disconnect clients upon a failed handshake. It waits for the
                // client to disconnect or to retry the handshake
                self.parsers.handshake.reset();
                
                // Send handsake to the client
                self.queue_raw_cmd_write(&response[0..size]).ok();

                // If the server selected a protocol version, valid handshake complete
                if size != HandshakeGenerator::size_for_num_versions(0){
                    self.do_handshook();
                } else {
                    warn!("Client sent invalid versions during handshaking.");
                }
            },
            None => { /* Not done parsing handshake yet */ } 
        }
    }

    /// Handle a byte read from the command port after handshaking
    fn handle_cmd_byte_normal(&mut self, b: &u8) {
        // TODO: Parse
    }

    /// Handle bytes read from the command port
    fn handle_cmd_bytes(&mut self, data: &[u8]) {
        for b in data {
            match self.state {
                State::Connected => self.handle_cmd_byte_handshake(b),
                State::Handshook => self.handle_cmd_byte_normal(b),
                _ => { /* Shouldn't ever get here */ }
            }
        }
    }

    /// Handle bytes read from the net table port
    fn handle_nt_bytes(&mut self, data: &[u8]) {
        // TODO: Parse 
    }

    /// Handle bytes read from the log port
    fn handle_log_bytes(&mut self, data: &[u8]) {
        // The client (DS) should never send data to the server (robot) on the log port
        // Log message are only sent from robot (server) to client (DS)
        warn!("Data received on log port, client is misbehaving!");
    }


    // =============================================================================================
    // Writing to clients
    // =============================================================================================

    /// Queue a complete message for write. This function **is** thread safe.
    /// Parameters:
    ///     data: Complete message. Must be complete or thread safety issues can occur mixing up
    ///           messages
    fn queue_raw_cmd_write(&self, data: &[u8]) -> io::Result<()> {
        match self.state {
            State::Connected|State::Handshook => {
                // Queue data to be written
                // NOTE: Implemented in Connected state even though it is not expected to be written
                // here since writes to command port are not actually disabled by server in
                // connected state. Just not used
                return self.wbuffer_cmd.queue(data);
            }
            _ => {
                // We should never get here. The only thing written over the command port is the
                // handshake response written by the server
                warn!("Attempted write to command port in an invalid server state!");
                return Err(io::Error::new(io::ErrorKind::NotConnected, 
                            "Command port is not ready to write"));
            }
        }
    }

    // TODO public functions to queue data on net table and log ports


    // =============================================================================================
    // State transitions
    // =============================================================================================

    /// Transition to State::Disconnected
    fn do_disconnect(&mut self) {
        debug!("Starting transition to Disconnected state.");

        // Stop polling clients before dropping the stream objects
        match self.state{
            State::Connected => {
                // When transitioning into connected state, the server will poll only on the command
                // port so it can process handshakes
                if let Some(client_cmd) = &mut self.client.cmd {
                    self.poll.registry().deregister(client_cmd).ok();
                }
            },
            State::Handshook => {
                // When transitioning into handshook state, the server will start polling all
                // clients and the UDP controller port
                if let Some(client_cmd) = &mut self.client.cmd {
                    self.poll.registry().deregister(client_cmd).ok();
                }
                if let Some(client_nt)  = &mut self.client.nt{
                    self.poll.registry().deregister(client_nt).ok();
                }
                if let Some(client_log) = &mut self.client.log {
                    self.poll.registry().deregister(client_log).ok();
                }
                // TODO: Controller port?
            },
            _ => {
                // deregister on something that was never registered results in undefined behavior
            }
        }

        // Disconnect any clients still connected
        drop(self.client.cmd.take());
        drop(self.client.nt.take());
        drop(self.client.log.take());
        self.client.ip = None;

        // State change
        debug!("Server in Disconnected state.");
        self.state = State::Disconnected;
    }

    /// Transition to State::Connecting
    fn do_connecting(&mut self){
        debug!("Starting transition to Connecting state.");

        if self.client.ip.is_none(){
            error!("Client IP was unset during transition to connecting state");
            self.do_disconnect();
        }

        // Since the first port to connect could be the command port, make sure the handshake
        // parser is clean and ready for reuse before entering this state
        self.parsers.handshake.reset();

        // Clear write buffers when a new client starts connecting to ensure previously in-flight
        // data will not be sent.
        self.wbuffer_cmd.reset();
        self.wbuffer_nt.reset();
        self.wbuffer_log.reset();

        // TODO: Clear read buffers (parsers)
        
        // Do state change
        debug!("Server in Connecting state");
        self.state = State::Connecting;  
    }

    /// Transition to State::Connected
    fn do_connected(&mut self) {
        debug!("Starting transition to Connected state.");
        
        // Start polling **only** the command client. We do not want to read from other ports
        // until after handshake is complete. Since we cannot ignore polled events safely with mio,
        // do not start polling until we are ready to handle such events on other ports.
        let Some(client_cmd) = &mut self.client.cmd else {
            error!("Client was missing during transition to connected state");
            self.do_disconnect();
            return;
        };
        if let Err(_) = self.poll.registry().register(client_cmd, Tokens::STREAM_CMD, Interest::READABLE) {
            error!("Failed to setup to poll command client. Disconnecting.");
            self.do_disconnect();
            return;
        }

        // State change (ONLY SET AFTER EVERYTHING IS REGISTERED or calls to do_disconnect will
        // attempt to unregister incorrectly)
        debug!("Server in Connected state");
        info!("Client connected. Waiting for handshake");
        self.state = State::Connected;
    }

    /// Transition to State::Handshook
    fn do_handshook(&mut self) {
        debug!("Starting transition to Handshook state.");
        info!("Handshake with client completed.");
        
        let (Some(client_cmd), Some(client_nt), Some(client_log)) = 
                (&mut self.client.cmd, &mut self.client.nt, &mut self.client.log) else {
            error!("Client was missing during transition to connected state");
            self.do_disconnect();
            return;
        };
        if let Err(_) = self.poll.registry().register(client_cmd, Tokens::STREAM_CMD, Interest::READABLE) {
            error!("Failed to setup to poll command client. Disconnecting.");
            self.do_disconnect();
            return;
        }
        if let Err(_) = self.poll.registry().register(client_nt,  Tokens::STREAM_NT,  Interest::READABLE) {
            error!("Failed to setup to poll NT client. Disconnecting.");
            self.poll.registry().deregister(client_cmd).ok();
            self.do_disconnect();
            return;
        }
        if let Err(_) = self.poll.registry().register(client_log, Tokens::STREAM_LOG, Interest::READABLE) {
            error!("Failed to setup to poll log client. Disconnecting.");
            self.poll.registry().deregister(client_log).ok();
            self.poll.registry().deregister(client_nt).ok();
            self.do_disconnect();
            return;
        }
        
        // TODO: Start polling UDP port

        // State change (ONLY SET AFTER EVERYTHING IS REGISTERED or calls to do_disconnect will
        // attempt to unregister incorrectly)
        debug!("Server in Handshook state");
        self.state = State::Handshook;
    }
}
