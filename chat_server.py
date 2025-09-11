#!/usr/bin/env python3
"""
IntranetChat Server - A local network chat server using sockets
"""

import socket
import threading
import time
from datetime import datetime

class ChatServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.clients = {}  # {socket: username}
        self.socket = None
        self.running = False
        
    def start(self):
        """Start the chat server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind((self.host, self.port))
            self.socket.listen(5)
            self.running = True
            
            print(f"Chat server started on {self.host}:{self.port}")
            print("Waiting for connections...")
            
            while self.running:
                try:
                    client_socket, client_address = self.socket.accept()
                    print(f"New connection from {client_address}")
                    
                    # Start a new thread to handle the client
                    client_thread = threading.Thread(
                        target=self.handle_client,
                        args=(client_socket, client_address)
                    )
                    client_thread.daemon = True
                    client_thread.start()
                    
                except socket.error as e:
                    if self.running:
                        print(f"Error accepting connection: {e}")
                        
        except Exception as e:
            print(f"Error starting server: {e}")
        finally:
            self.stop()
    
    def handle_client(self, client_socket, client_address):
        """Handle communication with a connected client"""
        username = None
        try:
            # Request username from client
            client_socket.send("USERNAME".encode())
            username = client_socket.recv(1024).decode().strip()
            
            if not username:
                username = f"User_{client_address[1]}"
                
            self.clients[client_socket] = username
            
            # Notify all clients about new user
            join_message = f"{username} joined the chat"
            self.broadcast_message(join_message, exclude_client=client_socket)
            
            # Send welcome message to the new user
            welcome_msg = f"Welcome to IntranetChat, {username}! Type your messages below."
            client_socket.send(welcome_msg.encode())
            
            print(f"{username} ({client_address}) joined the chat")
            
            # Listen for messages from this client
            while self.running:
                try:
                    message = client_socket.recv(1024).decode().strip()
                    if not message:
                        break
                        
                    if message.lower() == '/quit':
                        break
                        
                    # Broadcast the message to all clients
                    timestamp = datetime.now().strftime("%H:%M:%S")
                    formatted_message = f"[{timestamp}] {username}: {message}"
                    self.broadcast_message(formatted_message, exclude_client=client_socket)
                    
                except socket.error:
                    break
                    
        except Exception as e:
            print(f"Error handling client {client_address}: {e}")
        finally:
            # Clean up when client disconnects
            if client_socket in self.clients:
                username = self.clients[client_socket]
                del self.clients[client_socket]
                
                # Notify other clients about user leaving
                leave_message = f"{username} left the chat"
                self.broadcast_message(leave_message)
                
                print(f"{username} ({client_address}) left the chat")
                
            client_socket.close()
    
    def broadcast_message(self, message, exclude_client=None):
        """Send message to all connected clients"""
        disconnected_clients = []
        
        for client_socket in list(self.clients.keys()):
            if client_socket != exclude_client:
                try:
                    client_socket.send(message.encode())
                except socket.error:
                    disconnected_clients.append(client_socket)
        
        # Remove disconnected clients
        for client in disconnected_clients:
            if client in self.clients:
                username = self.clients[client]
                del self.clients[client]
                print(f"Removed disconnected client: {username}")
                client.close()
    
    def stop(self):
        """Stop the chat server"""
        self.running = False
        
        # Close all client connections
        for client_socket in list(self.clients.keys()):
            try:
                client_socket.close()
            except:
                pass
        self.clients.clear()
        
        # Close server socket
        if self.socket:
            try:
                self.socket.close()
            except:
                pass
                
        print("Chat server stopped")

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='IntranetChat Server')
    parser.add_argument('--host', default='localhost', help='Server host address (default: localhost)')
    parser.add_argument('--port', type=int, default=12345, help='Server port (default: 12345)')
    
    args = parser.parse_args()
    
    server = ChatServer(args.host, args.port)
    
    try:
        server.start()
    except KeyboardInterrupt:
        print("\nShutting down server...")
        server.stop()

if __name__ == "__main__":
    main()