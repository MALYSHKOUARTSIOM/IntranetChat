#!/bin/bash
# start_client.sh - Convenience script to start the chat client

echo "Starting IntranetChat Client..."
echo "Usage: ./start_client.sh [--host SERVER_IP] [--username YOUR_NAME]"
echo ""

# Check if Python 3 is available
if command -v python3 &> /dev/null; then
    python3 chat_client.py "$@"
elif command -v python &> /dev/null; then
    python chat_client.py "$@"
else
    echo "Error: Python 3 is required but not found in PATH"
    exit 1
fi