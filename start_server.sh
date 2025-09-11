#!/bin/bash
# start_server.sh - Convenience script to start the chat server

echo "Starting IntranetChat Server..."
echo "To allow connections from other computers in the network, use: --host 0.0.0.0"
echo "Default: Server will start on localhost:12345"
echo ""

# Check if Python 3 is available
if command -v python3 &> /dev/null; then
    python3 chat_server.py "$@"
elif command -v python &> /dev/null; then
    python chat_server.py "$@"
else
    echo "Error: Python 3 is required but not found in PATH"
    exit 1
fi