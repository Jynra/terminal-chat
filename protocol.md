# Communication Protocol Documentation

## Overview

This document describes the communication protocol used in the Terminal Chat Application. The protocol is built on top of TCP/IP and uses a simple text-based message format.

## Connection Establishment

### Server Initialization
1. Server binds to `INADDR_ANY` on port 8085
2. Starts listening for incoming connections
3. Generates a unique 16-character alphanumeric key
4. Displays the key to be shared with clients

### Client Connection Process
1. Client connects to server IP:port
2. Sends authentication key (exactly 16 bytes)
3. Sends pseudonym (variable length, max 31 characters)
4. Server validates key
5. If valid, client is added to active clients list

## Message Format

### Authentication Messages

#### Key Exchange
```
[16 bytes] - Authentication key
```
- Fixed length: 16 characters
- Alphanumeric characters only
- No termination character

#### Pseudonym Exchange
```
[1-31 bytes] - User pseudonym
```
- Variable length (1-31 characters)
- UTF-8 encoded
- No specific termination required

### Chat Messages

#### Client to Server
```
[message content]
```
- Plain text message
- Maximum 1023 characters (BUFFER_SIZE - 1)
- Should not contain newlines

#### Server to Client
```
[username]:[message]
```
or
```
SYSTEM:[system message]
```

**Format Details:**
- `username`: Sender's pseudonym
- `:`: Separator character
- `message`: Message content
- `SYSTEM`: Special username for system messages

## Protocol Flow

### Successful Connection
```
Client                          Server
  |                               |
  |---- TCP Connection ---------->|
  |                               |
  |---- Auth Key (16 bytes) ----->|
  |                               |
  |---- Pseudonym ------------->  |
  |                               |
  |<--- Welcome/System Messages --|
  |                               |
  |<--- Chat Messages ----------- |
  |                               |
  |---- User Messages ----------->|
  |                               |
```

### Authentication Failure
```
Client                          Server
  |                               |
  |---- TCP Connection ---------->|
  |                               |
  |---- Invalid Key ------------>|
  |                               |
  |<--- Connection Closed -------|
```

## Message Types

### System Messages
- **Join notification**: `SYSTEM:[username] has joined the chat`
- **Leave notification**: `SYSTEM:[username] has left the chat`
- **Server messages**: Various status updates

### User Messages
- **Regular chat**: `[username]:[message content]`
- All user messages are prefixed with the sender's pseudonym

## Error Handling

### Connection Errors
- Invalid key results in immediate disconnection
- Network errors trigger client removal
- Server full (>10 clients) results in connection rejection

### Message Errors
- Malformed messages are ignored
- Empty messages are not broadcast
- Buffer overflow protection limits message size

## Technical Implementation Details

### Socket Configuration
- Protocol: TCP (SOCK_STREAM)
- Address Family: AF_INET (IPv4)
- Socket Options: SO_REUSEADDR enabled

### Threading Model
- Server uses one thread per client
- Clients use separate thread for receiving messages
- Mutex protection for shared server state

### Broadcast Mechanism
```c
// Server broadcasts to all clients except sender
for (each client) {
    if (client.active && client != sender) {
        send(client.socket, formatted_message, ...);
    }
}
```

## Security Considerations

### Authentication
- 16-character key provides ~62^16 possible combinations
- Key is generated using C's rand() function
- No encryption of messages in transit
- Keys are transmitted in plaintext

### Limitations
- No message encryption
- No user authentication beyond key
- No protection against eavesdropping
- Single shared key for all clients

## Wire Protocol Examples

### Successful Session
```
# Connection
Client -> Server: [16-byte key]
Client -> Server: "Alice"

# Chat exchange
Server -> Client: "SYSTEM:Alice has joined the chat"
Client -> Server: "Hello everyone!"
Server -> All: "Alice:Hello everyone!"
```

### Message Broadcasting
```
# When Alice sends a message
Alice -> Server: "How is everyone doing?"

# Server reformats and broadcasts
Server -> Bob: "Alice:How is everyone doing?"
Server -> Charlie: "Alice:How is everyone doing?"
# (Not sent back to Alice)
```

## Future Enhancements

Potential protocol improvements could include:
- Message encryption (TLS/SSL)
- User authentication
- Message timestamps
- Private messaging
- Message history
- Compression for large messages
- Heartbeat/keepalive mechanism