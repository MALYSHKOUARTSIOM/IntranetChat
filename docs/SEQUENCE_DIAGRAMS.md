# IntranetChat - Sequence Diagrams

## Overview
These diagrams visualize the interaction between the User, the Main Window Interface, and the low-level Network Classes over time.

## 1. Connection Sequence (Client Side)
How the application handles a user request to join a chat.

![Connection Sequence](/docs/schema/sequence_connect.svg)

### Flow:
1.  User initiates the connection.
2.  `MainWindow` instantiates a new `QTcpSocket`.
3.  The socket performs the TCP Handshake with the remote server.
4.  Upon success, Qt emits signals that update the UI state to "Connected".

## 2. Sending a Message
The process of transforming user input into a network packet.

![Sending Sequence](/docs/schema/sequence_send.svg)

### Flow:
1.  User submits text.
2.  The system captures the exact time.
3.  Data is encoded (Base64) to ensure transmission safety.
4.  The raw bytes are written to the socket.
5.  The UI updates immediately to show the user's own message, providing instant feedback (Optimistic UI).