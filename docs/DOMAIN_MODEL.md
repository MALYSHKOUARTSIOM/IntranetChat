# IntranetChat - Domain Model & Class Diagram

## Overview
This document describes the class structure of the IntranetChat application. Since the application is built using the Qt framework and C++, the domain model primarily consists of the main application class and its relationship with Qt's networking classes.

## Class Diagram Schema

![Class Diagram](/docs/schema/class_diagram.svg)

## Class Diagram Description

The core logic is encapsulated within the `MainWindow` class, which manages both the User Interface and the Network Logic.

### Key Entities

1.  **`MainWindow` (Class)**
    *   **Role:** The central controller and view of the application. It handles UI events (buttons, typing) and network signals.
    *   **Responsibility:**
        *   Managing the UI state (Client vs Server mode).
        *   Persisting user preferences (Nickname).
        *   Processing incoming TCP data.
        *   Broadcasting messages in Server mode.

2.  **`QTcpServer` (Qt Class)**
    *   **Role:** TCP Server implementation.
    *   **Relationship:** `MainWindow` *owns* one `QTcpServer` instance (composition) when in Server mode.
    *   **Responsibility:** Listening on a specific port and accepting incoming `QTcpSocket` connections.

3.  **`QTcpSocket` (Qt Class)**
    *   **Role:** TCP Socket implementation.
    *   **Relationship:**
        *   **Client Mode:** `MainWindow` owns 1 `QTcpSocket` to communicate with the host.
        *   **Server Mode:** `MainWindow` manages a list (`QList`) of `QTcpSocket*` representing all connected clients.
    *   **Responsibility:** Establishing connections and transmitting binary data.

### Relationships & Multiplicity

*   **MainWindow — QTcpServer (1 : 0..1)**
    *   A `MainWindow` may have one active server instance if the user chooses "Server Mode".

*   **MainWindow — QTcpSocket (1 : 0..*)**
    *   In **Client Mode**, the window holds **1** socket connection to the server.
    *   In **Server Mode**, the window holds **Many** socket connections (one for each connected user) in the `m_clients` list.

### Attributes & Methods (MainWindow)

Based on `mainwindow.h`:

*   **Attributes:**
    *   `ui`: Pointer to the User Interface definition.
    *   `m_server`: Pointer to `QTcpServer` (Server logic).
    *   `m_socket`: Pointer to `QTcpSocket` (Client logic).
    *   `m_clients`: `QList<QTcpSocket*>` (List of connected peers).
    *   `m_buffers`: `QHash<QTcpSocket*, QByteArray>` (Data buffers for partial packet handling).

*   **Key Methods (Slots):**
    *   `handleCreateServer()`: Initializes `m_server`.
    *   `handleConnectToServer()`: Initializes `m_socket`.
    *   `handleSendMessage()`: Encodes and writes data to socket(s).
    *   `onSocketReadyRead()`: Reads incoming data from sockets.
    *   `processBuffer()`: Parses the raw bytes into `timestamp:nickname:message` format.