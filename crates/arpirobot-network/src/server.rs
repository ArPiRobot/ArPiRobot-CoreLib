//! This module implements the server side communication

use mio::net::{TcpListener};
use mio::{Events, Interest, Poll, Token};
use std::net::SocketAddr;
use std::io::Result;

// Tokens to identify which listener an event belongs to when polling
const CTRL_TOKEN: Token = Token(0);
const CMD_TOKEN: Token = Token(1);
const NT_TOKEN: Token = Token(2);
const LOG_TOKEN: Token = Token(3);


/// Server implementation
pub struct Server {
    connected: bool,

    // TCP server instances
    // TODO: UDP listener
    cmd_listener: TcpListener,
    nt_listener: TcpListener,
    log_listener: TcpListener
}

impl Server {
    /// Create a new server instance
    /// Arguments:
    /// - ctrl_addr: Address to listen on for controller UDP datagrams
    /// - cmd_addr:  Address to listen on for command TCP connections
    /// - nt_addr:   Address to listen on for net table TCP connections
    /// - log_addr:  Address to listen on for log TCP connections
    pub fn new(ctrl_addr: SocketAddr, cmd_addr: SocketAddr, nt_addr: SocketAddr, log_addr: SocketAddr) -> Result<Self> {
        return Ok(Self {
            connected: false,
            // TODO: ctrl listener
            cmd_listener: TcpListener::bind(cmd_addr)?,
            nt_listener: TcpListener::bind(nt_addr)?,
            log_listener: TcpListener::bind(log_addr)?
        })
    }
    
    // TODO: ctrl_addr
    
    /// Get address of command server
    pub fn cmd_addr(&self) -> Result<SocketAddr> {
        return self.cmd_listener.local_addr();
    }

    /// Get address of net table server
    pub fn nt_addr(&self) -> Result<SocketAddr> {
        return self.nt_listener.local_addr();
    }
    
    /// Get address of log server
    pub fn log_addr(&self) -> Result<SocketAddr> {
        return self.log_listener.local_addr();
    }

    /// Process one set of events on the servers (non-blocking multiplexed IO)
    /// This function **WILL** return. Looping and threading is left up to the caller!
    // TODO: Timeout???
    pub fn process(&self) {
        if self.connected {
            // When connection is fully established, poll connected clients. Additional connection
            // attempts will be ignored. Only one client should be allowed to connect at a time
            // TODO: Poll 
        } else {
            // When connection is not fully established, poll servers for connections to accept
            // TODO: Poll
        }
    }
}
