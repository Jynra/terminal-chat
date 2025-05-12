# Usage Guide - Terminal Chat Application

## Getting Started

### Prerequisites

Before using the chat application, ensure you have:
- A Unix-like operating system (Linux, macOS, etc.)
- GCC compiler
- GNU Readline library (`libreadline-dev` on Ubuntu/Debian)
- pthread library (usually included with GCC)

### Installation

1. Clone or download the project
2. Compile using make:
```bash
make
```
3. The executable `chat_app` will be created

## Step-by-Step Usage

### 1. Launching the Application

```bash
./chat_app
```

You'll see a welcome screen with the application banner and menu:
```
=======================================================
|                Terminal Chat App                    |
=======================================================

+-------------------------+
|   Choose your option:   |
+-------------------------+
|  1 - Start Server        |
|  2 - Connect to Server   |
+-------------------------+

Enter your choice (1 or 2):
```

### 2. Starting a Server

Choose option `1` to start a server:

1. The server will generate a unique 16-character key
2. Note this key - you'll need to share it with clients
3. The server is now ready to accept connections

Example server startup:
```
[2025-05-12 10:15:30] Server started with key: AB123456789CDEF0
[2025-05-12 10:15:30] Waiting for connections on port 8085...
[2025-05-12 10:15:30] Share this key with clients: AB123456789CDEF0
```

### 3. Connecting as a Client

Choose option `2` to connect to a server:

#### Step 1: Enter Server Key
```
Enter server key: AB123456789CDEF0
```
Type the key provided by the server host.

#### Step 2: Enter Server IP
```
Enter server IP address: 192.168.1.100
```
- Enter the server's IP address
- Press Enter for default (127.0.0.1/localhost)
- For remote connections, use the server's external IP

#### Step 3: Choose Your Pseudonym
```
Enter your pseudo: Alice
```
Choose a unique name (up to 31 characters).

### 4. Chat Interface

Once connected, you'll see the chat interface:

```
=======================================================
|                Terminal Chat App                    |
=======================================================

▶ Connected successfully!
▶ Server: 192.168.1.100:8085
▶ Type your messages below (Ctrl+C to exit)

                        Bob has joined the chat

Bob: Hello everyone!

    Alice (You): Hi Bob! How are you?

                    Charlie has joined the chat

Charlie: Hey folks!

-------------------------------------------------
Alice (You): |
```

## Interface Elements

### Message Display

1. **System Messages** (centered):
   - Yellow text
   - Notifications like "User has joined"
   - Server status messages

2. **Other Users' Messages** (left-aligned):
   - Cyan username
   - Format: `Username: message`

3. **Your Messages** (right-aligned):
   - Green username with "(You)"
   - Format: `Username (You): message`

### Input Area

- Located at the bottom
- Green prompt showing your name
- Supports GNU Readline features:
  - Command history (↑/↓ arrows)
  - Line editing (Ctrl+A, Ctrl+E, etc.)

## Commands and Controls

### Normal Chat
- Type your message and press Enter
- No special commands required

### Exiting
- **Ctrl+C**: Clean exit
- **Ctrl+D**: EOF signal (alternative exit)

## Advanced Usage

### Network Configuration

For connections across different networks:

1. **Port Forwarding**: Configure your router to forward port 8085
2. **Firewall**: Ensure port 8085 is open
3. **Public IP**: Use your public IP address for external connections

### Multiple Servers

You can run multiple servers by:
1. Using different keys for each server
2. Modifying the port number in `network.h`
3. Recompiling with `make re`

## Troubleshooting

### Connection Issues

**Problem**: Can't connect to server
```
[2025-05-12 10:20:15] Failed to connect to 192.168.1.100:8085
```
**Solutions**:
- Verify the server is running
- Check IP address and port
- Test network connectivity (`ping`)
- Ensure firewall allows connections

**Problem**: Invalid key
```
[2025-05-12 10:20:15] Client with invalid key tried to connect
```
**Solutions**:
- Double-check the server key
- Ensure no extra spaces or characters
- Get a fresh key from the server

### Display Issues

**Problem**: Garbled display or colors not working
**Solutions**:
- Ensure terminal supports ANSI colors
- Check terminal size (minimum 80 columns recommended)
- Try a different terminal emulator

### Performance Issues

**Problem**: Messages appear slowly
**Solutions**:
- Check network latency
- Reduce other network activity
- Ensure adequate system resources

## Best Practices

### For Server Hosts

1. **Share Keys Securely**: Provide keys through secure channels
2. **Monitor Activity**: Watch for suspicious connections
3. **Resource Management**: Restart server periodically for long sessions

### For Clients

1. **Meaningful Pseudonyms**: Choose names that identify you
2. **Respectful Communication**: Be mindful of others
3. **Clean Exit**: Use Ctrl+C rather than killing the process

## Limitations

Current limitations to be aware of:

1. **No Message History**: Messages don't persist after disconnect
2. **No Private Messages**: All messages are public
3. **No File Transfer**: Text-only communication
4. **No Message Encryption**: Messages sent in plaintext
5. **Limited to 10 Clients**: Maximum simultaneous connections

## Examples

### Local Network Chat
Server (192.168.1.100):
```bash
./chat_app
# Choose 1, get key ABC123...
```

Client:
```bash
./chat_app
# Choose 2
# Key: ABC123...
# IP: 192.168.1.100
# Pseudo: YourName
```

### Internet Chat
Server (with public IP):
```bash
./chat_app
# Choose 1
# Configure port forwarding for 8085
# Share public IP and key
```

Client:
```bash
./chat_app
# Choose 2
# Key: [server key]
# IP: [server public IP]
# Pseudo: YourName
```

## Tips and Tricks

1. **Readline Features**:
   - Use Tab for autocompletion (limited support)
   - Ctrl+A: Beginning of line
   - Ctrl+E: End of line
   - Ctrl+L: Clear screen

2. **Quick Local Testing**:
   - Run server in one terminal
   - Connect with 127.0.0.1 in another

3. **Network Discovery**:
   - Use `ifconfig` or `ip addr` to find your IP
   - Use `nmap` to scan for open ports

## Frequently Asked Questions

**Q: Can I change the port number?**
A: Yes, edit `PORT` in `include/network.h` and recompile.

**Q: How many people can chat at once?**
A: Maximum 10 clients plus the server.

**Q: Are messages saved?**
A: No, messages are not persistent.

**Q: Can I use this over the internet?**
A: Yes, with proper port forwarding and firewall configuration.

**Q: Is the chat secure?**
A: Messages are sent in plaintext. Don't share sensitive information.