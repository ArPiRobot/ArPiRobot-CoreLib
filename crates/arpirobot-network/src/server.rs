//! This module implements the server side communication

use mio::net::{TcpListener, TcpStream};
use mio::{Events, Interest, Poll, Token};
use std::net::{IpAddr, SocketAddr};
use std::io;
use std::time::Duration;

/// Tokens to identify what a polled event originates from
struct TokenId;
impl TokenId {
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

// Separate client info so Server struct can be destructured as needed 
struct Client {
    // Connection state
    pub connected: bool,
    pub ip: Option<IpAddr>,
    pub handshake: bool,

    // TCP Sockets for each client
    pub cmd: Option<TcpStream>,
    pub nt: Option<TcpStream>,
    pub log: Option<TcpStream>,
}

// Separate listeners for struct destructuring
struct Listener {
    // TODO: UDP listener
    cmd: TcpListener,
    nt: TcpListener,
    log: TcpListener,
}

// Server implementation
pub struct Server {
    // Client connection state
    client: Client,
    
    // Listener instances
    listener: Listener,

    // Polling
    poll: Poll,
    events: Events,
}

impl Server {
    /// Create a new server instance
    /// Arguments:
    /// - ctrl_addr: Address to listen on for controller UDP datagrams
    /// - cmd_addr:  Address to listen on for command TCP connections
    /// - nt_addr:   Address to listen on for net table TCP connections
    /// - log_addr:  Address to listen on for log TCP connections
    pub fn bind(ctrl_addr: SocketAddr, cmd_addr: SocketAddr, nt_addr: SocketAddr, log_addr: SocketAddr) -> io::Result<Self> {
        // Create network listeners (servers)
        let mut cmd_listener = TcpListener::bind(cmd_addr)?;
        let mut nt_listener = TcpListener::bind(nt_addr)?;
        let mut log_listener = TcpListener::bind(log_addr)?;

        // Listeners should always be polled for readable events. On TCP listeners, this indicates
        // an incoming connection. For UDP listeners TODO
        let poll = Poll::new()?;
        poll.registry().register(&mut cmd_listener, TokenId::LISTENER_CMD, Interest::READABLE)?;
        poll.registry().register(&mut nt_listener, TokenId::LISTENER_NT, Interest::READABLE)?;
        poll.registry().register(&mut log_listener, TokenId::LISTENER_LOG, Interest::READABLE)?;

        return Ok(Self {
            client: Client {
                connected: false,
                ip: None,
                handshake: false,
                cmd: None,
                nt: None,
                log: None,
            },
            listener: Listener { 
                cmd: cmd_listener,
                nt: nt_listener,
                log: log_listener,
            },
            poll: poll,
            events: Events::with_capacity(128),
        });
    }
    
    // TODO: Address getter

    /// Process one set of events on the servers (non-blocking multiplexed IO)
    /// This function **WILL** return. Looping and threading is left up to the caller!
    pub fn process(&mut self, duration: Option<Duration>) -> io::Result<()>{
        // Poll all listeners and clients (if fully connected) for events
        self.poll.poll(&mut self.events, duration)?;
        for event in self.events.iter() {
            match event.token() {
                TokenId::LISTENER_CMD => {
                    Self::handle_incoming_connection(&mut self.client.cmd, &self.listener.cmd)?;
                    Self::handle_connections_changed(&mut self.client);
                },
                TokenId::LISTENER_NT  => {
                    Self::handle_incoming_connection(&mut self.client.nt, &self.listener.nt)?
                },
                TokenId::LISTENER_LOG => {
                    Self::handle_incoming_connection(&mut self.client.log, &self.listener.log)?
                },
                TokenId::LISTENER_CTRL => {
                    // TODO: Ctrl port
                }
                TokenId::STREAM_CMD | TokenId::STREAM_NT | TokenId::STREAM_LOG => {
                   // TODO: Handle messages from clients 
                }
                _ => {
                    // Ignore unknown events
                }
            }
        }

        return Ok(());
    }

    fn handle_incoming_connection(stream: &mut Option<TcpStream>, listener: &TcpListener) -> io::Result<()> {
        // Loop to handle in case multiple clients are waiting
        loop{
            match listener.accept() {
                Ok((s, _a)) => {
                    if stream.is_none() {
                        // Store connection in the given stream
                        *stream = Some(s); 
                    } else {
                        // There was already a client connected to this port. Only accept one client
                        // at a time, so disconnect the new client.
                        drop(s);
                    }
                    return Ok(());
                }
                Err(e) => match e.kind() {
                    io::ErrorKind::WouldBlock => return Ok(()), // No connection waiting
                    _ => return Err(e)
                }
            }
        }
    }

    fn handle_connections_changed(client: &mut Client) {
        if client.connected {
            // Check for disconnects
            // TODO
        } else {
            // Check if a valid connection has been made
            let Some(ref client_cmd) = client.cmd else { return };
            let Some(ref client_nt)  = client.nt else  { return };
            let Some(ref client_log) = client.log else { return };
            let Ok(ref addr_cmd)       = client_cmd.peer_addr() else { return };
            let Ok(ref addr_nt)        = client_nt.peer_addr()  else { return };
            let Ok(ref addr_log)       = client_log.peer_addr() else { return };
            if (addr_cmd.ip() == addr_nt.ip()) && (addr_cmd.ip() == addr_log.ip()) {
                // Same client is connected to all ports
                // Start polling for client info too
                // Set client state info
                client.ip = Some(addr_cmd.ip());
                client.handshake = false; // Will wait for client to initiate handshake
                client.connected = true;
            }
        }
    }
}
