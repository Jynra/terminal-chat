# Terminal Chat Application

A simple terminal-based chat application that allows communication between machines even when they're not on the same local network.

## Features

- Server/Client architecture
- Secure key-based connection
- Multiple client support
- Cross-network communication
- Simple text-based interface

## Usage

### Compilation

To compile the application, simply run:

`make`

This will generate the executable `chat_app` in the project's root directory.

### Running the Application

To run the application:

`./chat_app`

You will be prompted to choose between:
1. Starting a server
2. Connecting to an existing server

If you choose to start a server, a unique key will be generated. Share this key with users who want to connect to your server.

If you choose to connect to a server, you'll need to enter:
- The server key
- The server's IP address
- Your pseudonym

## Dependencies

- C compiler (GCC recommended)
- POSIX threads library
- Standard C libraries

## Notes

- The application uses TCP/IP sockets for communication
- Default port: 8080
- Maximum pseudonym length: 32 characters
- Maximum simultaneous connections: 10