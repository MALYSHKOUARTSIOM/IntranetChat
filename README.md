# IntranetChat

A simple local network chat application using Python sockets. This application allows multiple users to chat with each other over a local network without requiring an internet connection.

## Features

- **Local Network Communication**: Works entirely within your local network
- **Multi-client Support**: Multiple users can join the same chat room
- **Real-time Messaging**: Messages are broadcast instantly to all connected users
- **User Identification**: Each user has a unique username
- **Simple CLI Interface**: Easy-to-use command-line interface
- **Connection Management**: Automatic handling of user joins/leaves
- **Cross-platform**: Works on Windows, macOS, and Linux

## Requirements

- Python 3.6 or higher
- No additional dependencies required (uses only built-in Python libraries)

## Quick Start

### 1. Start the Server

First, start the chat server on one machine in your network:

```bash
python3 chat_server.py
```

By default, the server listens on `localhost:12345`. To make it accessible from other machines in the network:

```bash
python3 chat_server.py --host 0.0.0.0 --port 12345
```

### 2. Connect Clients

On the same machine or other machines in the network, run the client:

```bash
python3 chat_client.py --host <server-ip> --username <your-name>
```

If connecting from the same machine as the server:

```bash
python3 chat_client.py --username Alice
```

If connecting from another machine in the network:

```bash
python3 chat_client.py --host 192.168.1.100 --username Bob
```

## Usage Examples

### Example 1: Local Chat on Same Computer

Terminal 1 (Server):
```bash
python3 chat_server.py
```

Terminal 2 (Client 1):
```bash
python3 chat_client.py --username Alice
```

Terminal 3 (Client 2):
```bash
python3 chat_client.py --username Bob
```

### Example 2: Network Chat Across Multiple Computers

On Computer 1 (Server):
```bash
python3 chat_server.py --host 0.0.0.0
```

On Computer 2 (Client):
```bash
python3 chat_client.py --host 192.168.1.100 --username Alice
```

On Computer 3 (Client):
```bash
python3 chat_client.py --host 192.168.1.100 --username Bob
```

## Command Line Options

### Server Options

```bash
python3 chat_server.py [options]

Options:
  --host HOST     Server host address (default: localhost)
                  Use 0.0.0.0 to accept connections from any network interface
  --port PORT     Server port (default: 12345)
```

### Client Options

```bash
python3 chat_client.py [options]

Options:
  --host HOST       Server host address (default: localhost)
  --port PORT       Server port (default: 12345)
  --username NAME   Your username for the chat
```

## Chat Commands

- Type any text and press Enter to send a message
- Type `/quit` to leave the chat
- Use Ctrl+C to exit

## Network Configuration

To find your computer's IP address for network chatting:

### Windows
```cmd
ipconfig
```

### macOS/Linux
```bash
ifconfig
# or
ip addr show
```

Look for your local network IP address (usually starts with 192.168.x.x or 10.x.x.x).

## Troubleshooting

### Common Issues

1. **"Connection refused" error**
   - Make sure the server is running
   - Check that you're using the correct IP address and port
   - Verify firewall settings allow connections on the specified port

2. **"Permission denied" error**
   - Try using a different port (e.g., 8080, 9000)
   - On some systems, ports below 1024 require administrator privileges

3. **Can't connect from other computers**
   - Ensure the server is started with `--host 0.0.0.0`
   - Check firewall settings on the server machine
   - Verify all computers are on the same network

### Firewall Configuration

You may need to allow the application through your firewall:

**Windows Firewall:**
- Go to Windows Defender Firewall
- Click "Allow an app or feature through Windows Defender Firewall"
- Add Python or allow the specific port

**macOS Firewall:**
- Go to System Preferences > Security & Privacy > Firewall
- Add Python to the allowed applications

**Linux (ufw):**
```bash
sudo ufw allow 12345
```

## Technical Details

- **Protocol**: TCP sockets for reliable message delivery
- **Architecture**: Client-server model with message broadcasting
- **Threading**: Multi-threaded server to handle multiple clients simultaneously
- **Message Format**: Simple text-based protocol with timestamps
- **Error Handling**: Graceful handling of disconnections and network errors

## License

This project is open source and available under the MIT License.