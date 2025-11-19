# IntranetChat - State Diagrams

## Overview
This document describes the lifecycle of the network connection within the application, specifically focusing on the `QTcpSocket` states.

## Socket State Machine

The application relies on Qt's asynchronous network signals. The `MainWindow` reacts to transitions between these states to update the UI (e.g., enabling/disabling buttons).

![Socket State Diagram](/docs/schema/state_socket.svg)

### States Description:

1.  **Unconnected:** The initial state. No connection exists. The "Connect" button is enabled.
2.  **HostLookup:** The socket is performing a DNS lookup (or parsing the IP string).
3.  **Connecting:** The socket has located the target and is establishing the TCP handshake.
4.  **Connected:** The session is active. The "Connect" button is hidden, "Disconnect" is shown. The user can send messages.
5.  **Closing:** The server or client has initiated a shutdown sequence.