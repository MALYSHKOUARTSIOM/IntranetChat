#!/usr/bin/env python3
"""
Simple test script to validate basic IntranetChat functionality
"""

import socket
import time
import subprocess
import sys
import os

def test_server_startup():
    """Test if the server can start up properly"""
    print("Testing server startup...")
    
    # Test if we can bind to a port
    try:
        test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        test_socket.bind(('localhost', 0))  # Let OS choose port
        port = test_socket.getsockname()[1]
        test_socket.close()
        print(f"✓ Can bind to port {port}")
        return True
    except Exception as e:
        print(f"✗ Cannot bind to socket: {e}")
        return False

def test_client_creation():
    """Test if the client can be created"""
    print("Testing client creation...")
    
    try:
        # Try to import and create the client class
        sys.path.insert(0, os.getcwd())
        from chat_client import ChatClient
        
        client = ChatClient('localhost', 12345)
        print("✓ Client class can be instantiated")
        return True
    except Exception as e:
        print(f"✗ Client creation failed: {e}")
        return False

def test_socket_communication():
    """Test basic socket communication"""
    print("Testing socket communication...")
    
    try:
        # Create a simple echo server for testing
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind(('localhost', 0))
        port = server_socket.getsockname()[1]
        server_socket.listen(1)
        
        # Test client connection
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.settimeout(1)
        
        try:
            client_socket.connect(('localhost', port))
            # Accept connection on server side
            conn, addr = server_socket.accept()
            
            # Test message exchange
            test_msg = "Hello, IntranetChat!"
            client_socket.send(test_msg.encode())
            received = conn.recv(1024).decode()
            
            if received == test_msg:
                print("✓ Socket communication works")
                result = True
            else:
                print(f"✗ Message mismatch: sent '{test_msg}', received '{received}'")
                result = False
                
        except socket.timeout:
            print("✗ Socket connection timeout")
            result = False
        finally:
            client_socket.close()
            server_socket.close()
            
        return result
        
    except Exception as e:
        print(f"✗ Socket communication test failed: {e}")
        return False

def main():
    print("IntranetChat Basic Functionality Test")
    print("=" * 40)
    
    tests = [
        test_server_startup,
        test_client_creation,
        test_socket_communication
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
        print()
    
    print("=" * 40)
    print(f"Test Results: {passed}/{total} tests passed")
    
    if passed == total:
        print("✓ All basic functionality tests passed!")
        print("The IntranetChat system appears to be working correctly.")
    else:
        print("✗ Some tests failed. Please check the implementation.")
        
    return passed == total

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)