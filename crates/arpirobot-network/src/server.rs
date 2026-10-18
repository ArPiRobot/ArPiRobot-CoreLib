//! This module implements the server side communication

use mio::net::{TcpListener, TcpStream};
use mio::{Events, Interest, Poll, Token};
use std::net::{IpAddr, SocketAddr};
use std::io;
use std::time::Duration;
use log::{warn, error, info, debug};

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
    pub const LISTENER_NT: Token = Token(2);
    pub const LISTENER_LOG: Token = Token(3);

    // Client tokens
    pub const STREAM_CMD: Token = Token(4);
    pub const STREAM_NT: Token = Token(5);
    pub const STREAM_LOG: Token = Token(6);
}

struct Clients {
    // TCP Sockets for each client
    pub cmd: Option<TcpStream>,
    pub nt: Option<TcpStream>,
    pub log: Option<TcpStream>,

    // IP address for connected clients (remote side)
    pub ip: Option<IpAddr>,
}

struct Listeners {
    // TODO: UDP listener
    cmd: TcpListener,
    nt: TcpListener,
    log: TcpListener,
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
    events: Events
}

impl Server {
    /// Create a new server instance
    /// Arguments:
    /// - ctrl_addr: Address to listen on for controller UDP datagrams
    /// - cmd_addr:  Address to listen on for command TCP connections
    /// - nt_addr:   Address to listen on for net table TCP connections
    /// - log_addr:  Address to listen on for log TCP connections
    pub fn bind(ctrl_addr: SocketAddr, cmd_addr: SocketAddr, nt_addr: SocketAddr, log_addr: SocketAddr) -> io::Result<Self> {
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
            poll: Poll::new()?,
            events: Events::with_capacity(16),
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

    /// Process a set of polled events
    fn handle_events(&mut self, events: &Events) -> io::Result<()> {
        for event in events {
            let token = event.token();
            match token {
                Tokens::LISTENER_CMD | Tokens::LISTENER_NT | Tokens::LISTENER_LOG => {
                    self.handle_incoming_connection(token);
                }
                Tokens::LISTENER_CTRL => {
                    // TODO: Handle UDP data
                }
                Tokens::STREAM_CMD => {
                    self.handle_cmd_read();
                }
                Tokens::STREAM_NT => {
                    self.handle_nt_read();
                }
                Tokens::STREAM_LOG => {
                    self.handle_log_read();
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
    
    /// Handle readable events on the command port's tcp stream (client)
    fn handle_cmd_read(&mut self) {
        // TODO: Handle messages on the command port
    }

    /// Handle readable events on the net table port's tcp stream (client)
    fn handle_nt_read(&mut self) {
        // TODO: Handle messages on the net table port
    }

    /// Handle readable events on the log port's tcp stream (client)
    fn handle_log_read(&mut self) {
        // TODO: Handles messages on the log port
    }

    /// Transition to State::Disconnected
    fn do_disconnect(&mut self) {
        // Stop polling clients before dropping the stream objects
        match self.state{
            State::Connected | State::Handshook => {
                if let Some(client_cmd) = &mut self.client.cmd {
                    self.poll.registry().deregister(client_cmd).ok();
                }
                if let Some(client_nt)  = &mut self.client.nt{
                    self.poll.registry().deregister(client_nt).ok();
                }
                if let Some(client_log) = &mut self.client.log {
                    self.poll.registry().deregister(client_log).ok();
                }
            },
            _ => {
                // Calling deregister on something that was never registered results in undefined
                // behavior per mio docs. Client streams are only registered just before transition
                // to the Connected state.
            }
        }

        // Disconnect any clients still connected
        drop(self.client.cmd.take());
        drop(self.client.nt.take());
        drop(self.client.log.take());
        self.client.ip = None;

        // State change
        info!("Client disconnected");
        self.state = State::Disconnected;
    }

    /// Transition to State::Connecting
    fn do_connecting(&mut self){
        debug!("Client starting to connect");
        if self.client.ip.is_none(){
            error!("Client IP was unset during transition to connecting state");
            self.do_disconnect();
        }
        self.state = State::Connecting;  
    }

    /// Transition to State::Connected
    fn do_connected(&mut self) {
        // Start polling clients too
        let (Some(client_cmd), Some(client_nt), Some(client_log)) = 
                (&mut self.client.cmd, &mut self.client.nt, &mut self.client.log) else {
            error!("Client was missing during transition to connected state");
            self.do_disconnect();
            return;
        };
        if let Err(_) = self.poll.registry().register(client_cmd, Tokens::STREAM_CMD, Interest::READABLE) {
            error!("Failed to setup to poll client. Disconnecting.");
            self.do_disconnect();
            return;
        }
        if let Err(_) = self.poll.registry().register(client_nt,  Tokens::STREAM_NT,  Interest::READABLE) {
            error!("Failed to setup to poll client. Disconnecting.");
            self.poll.registry().deregister(client_cmd).ok();
            self.do_disconnect();
            return;
        }
        if let Err(_) = self.poll.registry().register(client_log, Tokens::STREAM_LOG, Interest::READABLE) {
            error!("Failed to setup to poll client. Disconnecting.");
            self.poll.registry().deregister(client_log).ok();
            self.poll.registry().deregister(client_nt).ok();
            self.do_disconnect();
            return;
        }

        // State change (ONLY SET AFTER EVERYTHING IS REGISTERED or calls to do_disconnect will
        // attempt to unregister incorrectly)
        self.state = State::Connected;
    }

    /// Transition to State::Handshook
    fn do_handshook(&mut self) {
        // State change
        self.state = State::Handshook;
    }
}
