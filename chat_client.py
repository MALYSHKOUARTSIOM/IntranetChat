#!/usr/bin/env python3
"""
IntranetChat Client - A local network chat client using sockets
"""

import socket
import threading
import sys
import time

class ChatClient:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.socket = None
        self.username = None
        self.connected = False
        self.running = False
        
    def connect(self, username):
        """Connect to the chat server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.username = username
            self.connected = True
            self.running = True
            
            # Wait for username request from server
            initial_msg = self.socket.recv(1024).decode()
            if initial_msg == "USERNAME":
                self.socket.send(username.encode())
                
            # Start receiving messages in a separate thread
            receive_thread = threading.Thread(target=self.receive_messages)
            receive_thread.daemon = True
            receive_thread.start()
            
            return True
            
        except Exception as e:
            print(f"Error connecting to server: {e}")
            return False
    
    def receive_messages(self):
        """Receive messages from the server"""
        while self.running and self.connected:
            try:
                message = self.socket.recv(1024).decode()
                if message:
                    print(f"\r{message}")
                    print(f"{self.username}> ", end="", flush=True)
                else:
                    break
            except socket.error:
                break
                
        self.connected = False
    
    def send_message(self, message):
        """Send message to the server"""
        if self.connected and message.strip():
            try:
                self.socket.send(message.encode())
                return True
            except socket.error:
                self.connected = False
                return False
        return False
    
    def disconnect(self):
        """Disconnect from the server"""
        self.running = False
        self.connected = False
        
        if self.socket:
            try:
                self.socket.send("/quit".encode())
                self.socket.close()
            except:
                pass
                
        print("Disconnected from chat server")

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='IntranetChat Client')
    parser.add_argument('--host', default='localhost', help='Server host address (default: localhost)')
    parser.add_argument('--port', type=int, default=12345, help='Server port (default: 12345)')
    parser.add_argument('--username', help='Your username for the chat')
    
    args = parser.parse_args()
    
    # Get username if not provided
    if not args.username:
        args.username = input("Enter your username: ").strip()
        if not args.username:
            print("Username cannot be empty!")
            sys.exit(1)
    
    client = ChatClient(args.host, args.port)
    
    print(f"Connecting to chat server at {args.host}:{args.port}...")
    
    if client.connect(args.username):
        print(f"Connected successfully as '{args.username}'")
        print("Type your messages and press Enter to send.")
        print("Type '/quit' to exit the chat.")
        print("-" * 50)
        
        try:
            while client.connected:
                try:
                    # Get user input
                    message = input(f"{args.username}> ")
                    
                    if message.lower().strip() == '/quit':
                        break
                        
                    if not client.send_message(message):
                        print("Failed to send message. Connection lost.")
                        break
                        
                except KeyboardInterrupt:
                    break
                except EOFError:
                    break
                    
        except Exception as e:
            print(f"Error: {e}")
        finally:
            client.disconnect()
    else:
        print("Failed to connect to the chat server.")
        print("Make sure the server is running and the host/port are correct.")

if __name__ == "__main__":
    main()