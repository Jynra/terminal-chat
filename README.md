# Terminal Chat Application

A secure, multi-user terminal-based chat application with cross-network communication capabilities.

## Features

### 🔒 Security
- Unique key-based authentication system
- No passwords required - secure key sharing mechanism
- Automatic key generation for server hosts

### 🌐 Network Capabilities
- Server/Client architecture
- Cross-network communication (LAN/WAN)
- IPv4 support with configurable IP addresses
- Default port 8085 (configurable)

### 💬 Chat Features
- Multiple simultaneous connections (up to 10 clients)
- Real-time message broadcasting
- Colorized interface with user identification
- System messages for join/leave notifications
- Message alignment (own messages on right, others on left)
- GNU Readline support for better input handling

### 🎨 User Interface
- Clean terminal interface with ANSI color codes
- Bordered message display
- Terminal width adaptation
- Welcome banner and structured layout

## Quick Start

### Installation

```bash
# Clone the repository
git clone <repository-url>
cd terminal-chat-app

# Compile the application
make
```

### Running the Application

```bash
./chat_app
```

You'll be presented with two options:
1. **Start Server** - Create a new chat room
2. **Connect to Server** - Join an existing chat room

## Usage

### As a Server Host

1. Choose option `1` from the main menu
2. A unique 16-character key will be generated
3. Share this key with users who want to join
4. Server will display connection messages and chat activity

### As a Client

1. Choose option `2` from the main menu
2. Enter the server key (provided by the host)
3. Enter the server's IP address (or press Enter for localhost)
4. Choose your pseudonym
5. Start chatting!

## Technical Specifications

### Requirements
- GCC compiler with C99 support
- POSIX threads library (pthread)
- GNU Readline library
- Standard POSIX libraries

### Compilation Options
- Wall, Wextra, Werror for strict error checking
- pthread linking for multithreading support
- readline linking for enhanced input

### Key Technical Details
- **Protocol**: TCP/IP sockets
- **Default Port**: 8085
- **Max Clients**: 10 simultaneous connections
- **Key Length**: 16 characters (alphanumeric)
- **Pseudo Length**: Up to 31 characters
- **Buffer Size**: 1024 bytes per message

## File Structure

```
.
├── Makefile                            # Build configuration
├── README.md                           # Main documentation
├── include/                            # Header files
│   ├── client.h                       # Client functionality
│   ├── colors.h                       # ANSI color definitions
│   ├── network.h                      # Network operations
│   ├── server.h                       # Server functionality
│   └── utils.h                        # Utility functions
├── src/                               # Source files
│   ├── main.c                         # Application entry point
│   ├── client/                        # Client implementation
│   │   ├── client.c                   # Client core functions
│   │   └── client_handlers.c          # Message handling for client
│   ├── server/                        # Server implementation
│   │   ├── server.c                   # Server core functions
│   │   └── server_handlers.c          # Client connection handling
│   ├── network/                       # Network layer
│   │   ├── connection.c               # Connection management
│   │   └── socket.c                   # Socket operations
│   └── utils/                         # Utility functions
│       ├── display.c                  # Terminal display functions
│       ├── error_handler.c            # Error handling utilities
│       ├── key_generator.c            # Random key generation
│       └── logger.c                   # Logging functionality
└── docs/                              # Documentation
    ├── protocol.md                    # Communication protocol
    └── usage.md                       # Detailed usage guide
```

## Network Configuration

The application uses port **8085** by default. If you need to use a different port:

1. Edit `include/network.h`
2. Change the `PORT` definition
3. Recompile with `make re`

For firewall configuration, ensure port 8085 (or your custom port) is open for TCP traffic.

## Building

```bash
# Clean build
make clean
make

# Force rebuild
make re

# Clean all generated files
make fclean
```

## Compliance

This project follows the Jynra coding standards (jynra_norm.txt) ensuring:
- 80-column line limit
- 4-space tab indentation
- Consistent formatting
- Proper function separation

## Troubleshooting

### Common Issues

1. **Connection refused**
   - Check if server is running
   - Verify IP address and port
   - Ensure firewall allows connections

2. **Invalid key error**
   - Double-check the server key
   - Ensure key hasn't been mistyped

3. **Compilation errors**
   - Install readline development packages
   - Ensure GCC and pthread libraries are available

### Getting Help

If you encounter issues:
1. Check the logs for error messages
2. Verify network connectivity
3. Ensure all dependencies are installed

## Contributing

When contributing, please:
1. Follow the Jynra coding standards
2. Test thoroughly on both client and server
3. Update documentation for new features
4. Ensure cross-platform compatibility

## License
