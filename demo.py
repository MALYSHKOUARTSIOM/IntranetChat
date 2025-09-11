#!/usr/bin/env python3
"""
IntranetChat Demo - Demonstrates how to use the chat system
"""

import subprocess
import time
import sys
import os

def print_header(title):
    """Print a formatted header"""
    print("\n" + "=" * 60)
    print(f" {title}")
    print("=" * 60)

def print_step(step_num, description):
    """Print a step description"""
    print(f"\nStep {step_num}: {description}")
    print("-" * 40)

def main():
    print_header("IntranetChat Demo")
    
    print("This demo shows how to use the IntranetChat system.")
    print("The system consists of two main components:")
    print("1. chat_server.py - The server that manages chat rooms")
    print("2. chat_client.py - The client that users run to join chats")
    
    print_step(1, "Available Commands")
    print("\nServer commands:")
    print("  python3 chat_server.py                    # Start server on localhost:12345")
    print("  python3 chat_server.py --host 0.0.0.0     # Allow connections from network")
    print("  python3 chat_server.py --port 8080        # Use custom port")
    print("  ./start_server.sh                         # Use convenience script")
    
    print("\nClient commands:")
    print("  python3 chat_client.py --username Alice   # Connect with username")
    print("  python3 chat_client.py --host 192.168.1.100 --username Bob  # Connect to remote server")
    print("  ./start_client.sh --username Charlie      # Use convenience script")
    
    print_step(2, "Demo Scenario")
    print("\nTo test the chat system locally:")
    print("1. Open a terminal and run: python3 chat_server.py")
    print("2. Open another terminal and run: python3 chat_client.py --username Alice")
    print("3. Open a third terminal and run: python3 chat_client.py --username Bob")
    print("4. Start typing messages in the client terminals")
    print("5. Messages will appear in real-time across all connected clients")
    print("6. Type '/quit' to disconnect a client")
    print("7. Use Ctrl+C to stop the server")
    
    print_step(3, "Network Setup")
    print("\nTo chat across multiple computers:")
    print("1. Find the server computer's IP address:")
    print("   - Windows: ipconfig")
    print("   - macOS/Linux: ifconfig or ip addr")
    print("2. Start server with: python3 chat_server.py --host 0.0.0.0")
    print("3. On other computers, connect with: python3 chat_client.py --host <server-ip> --username <name>")
    
    print_step(4, "Example Network IPs")
    print("\nCommon local network IP ranges:")
    print("  • 192.168.1.x (most home routers)")
    print("  • 192.168.0.x (some home routers)")
    print("  • 10.x.x.x (corporate networks)")
    print("  • 172.16.x.x to 172.31.x.x (corporate networks)")
    
    print_step(5, "Troubleshooting")
    print("\nCommon issues and solutions:")
    print("  • 'Connection refused': Make sure server is running")
    print("  • 'Address already in use': Use a different port")
    print("  • Can't connect from other computers: Check firewall settings")
    print("  • 'Permission denied': Try a port number > 1024")
    
    print_step(6, "Features")
    print("\nCurrent features:")
    print("  ✓ Real-time messaging")
    print("  ✓ Multiple simultaneous users")
    print("  ✓ User join/leave notifications")
    print("  ✓ Message timestamps")
    print("  ✓ Graceful connection handling")
    print("  ✓ Cross-platform compatibility")
    
    print_header("Ready to Chat!")
    print("Start with: python3 chat_server.py")
    print("Then: python3 chat_client.py --username YourName")
    print("\nHappy chatting! 🎉")

if __name__ == "__main__":
    main()