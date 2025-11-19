# IntranetChat - Activity Diagrams

## Overview
This document illustrates the logic flow for key operations within the IntranetChat application using UML Activity Diagrams.

## 1. Server Startup Process
This diagram describes the flow executed when a user attempts to create a chat session (Host).

![Server Startup Activity](/docs/schema/activity_server.svg)

### Description:
1.  The system validates the input port number.
2.  The `QTcpServer` attempts to listen on the specified port.
3.  If successful, the system queries network interfaces to find the local IPv4 address.
4.  The UI is locked to prevent changing modes while the server is running.

## 2. Message Reception & Processing
This diagram details the algorithm for processing incoming TCP data packets (`processBuffer` function).

![Message Reception Activity](/docs/schema/activity_receive.svg)

### Description:
1.  Data is read from the socket into a persistent buffer (to handle TCP fragmentation).
2.  The buffer is checked for a newline character `\n`, acting as a packet delimiter.
3.  The packet is split into parts (Timestamp, Nickname, Body).
4.  Base64 decoding recovers the original text.
5.  **Server Logic:** If the app is running as a Server, it immediately broadcasts the valid packet to all other connected sockets so everyone stays in sync.