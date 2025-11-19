# Software Requirements Specification (SRS) for IntranetChat

---

# Table of Contents
1 [Introduction](#introduction)  
1.1 [Purpose](#purpose)  
1.2 [Business Requirements](#business-requirements)  
1.2.1 [Background](#background)  
1.2.2 [Project Scope](#project-scope)  
1.3 [Analogs](#analogs)  
2 [User Requirements](#user-requirements)  
2.1 [Software Interfaces](#software-interfaces)  
2.2 [User Interface](#user-interface)  
2.3 [User Characteristics](#user-characteristics)  
2.4 [Assumptions and Dependencies](#assumptions-and-dependencies)  
3 [System Requirements](#system-requirements)  
3.1 [Functional Requirements](#functional-requirements)  
3.1.1 [Core Functions](#core-functions)  
3.1.1.1 [Nickname Management](#nickname-mgmt)  
3.1.1.2 [Server Creation](#server-creation)  
3.1.1.3 [Client Connection](#client-connection)  
3.1.1.4 [Messaging](#messaging)  
3.2 [Non-Functional Requirements](#non-functional-requirements)  
3.2.1 [Quality Attributes](#quality-attributes)  
3.2.2 [System Constraints](#system-constraints)

<a name="introduction"/>

# 1 Introduction

<a name="purpose"/>

## 1.1 Purpose
This document describes the functional and non-functional requirements for the desktop application "IntranetChat" – a local network messenger built using C++ and the Qt framework.

<a name="business-requirements"/>

## 1.2 Business Requirements

<a name="background"/>

### 1.2.1 Background
In environments where internet access is restricted, unavailable, or monitored (e.g., secure offices, field operations, educational computer labs), users still require a method to communicate digitally. Existing solutions often require external servers or complex setups.

<a name="project-scope"/>

### 1.2.2 Project Scope
"IntranetChat" is a desktop application that allows users within the same Local Area Network (LAN) to exchange text messages. It operates in a peer-to-peer fashion where one user acts as the host (server). No internet connection is required.

<a name="analogs"/>

## 1.3 Analogs
*   **Skype / Telegram / WhatsApp:** Require active internet connection and external servers. IntranetChat works offline in LAN.
*   **NetSend (Old Windows utility):** Obsolete and command-line based. IntranetChat provides a modern GUI.
*   **LanTalk:** Commercial enterprise software. IntranetChat is a lightweight open-source alternative.

<a name="user-requirements"/>

# 2 User Requirements

<a name="software-interfaces"/>

## 2.1 Software Interfaces
*   **Language:** C++17.
*   **Framework:** Qt 6 (Modules: Widgets, Network).
*   **Communication Protocol:** Custom TCP-based text protocol using Base64 encoding.
*   **Settings Storage:** Local file system (`nickname.txt` in AppData).

<a name="user-interface"/>

## 2.2 User Interface
The User Interface is built using Qt Widgets with QSS styling for a "Dark Mode" look.

**1. Server Mode Configuration**
The user selects "Server" mode to host a chat. The system displays the local IP address.
![Server Mode Setup](/docs/mockups/server_mode.png)

**2. Client Mode Connection**
The user selects "Client" mode to join a chat by entering the Server's IP.
![Client Mode Setup](/docs/mockups/client_mode.png)

**3. Active Chat Window**
The main communication interface showing message history and input field.
![Active Chat Session](/docs/mockups/chat_process.png)

<a name="user-characteristics"/>

## 2.3 User Characteristics
The target audience includes office workers, students, or network administrators who need quick local communication. Users are expected to have basic computer literacy (understanding of IP addresses is helpful but not strictly mandatory for clients if provided by the host).

<a name="assumptions-and-dependencies"/>

## 2.4 Assumptions and Dependencies
1.  All devices are connected to the same router or switch (same subnet).
2.  Firewalls on host machines allow traffic on the selected port (default: 12345).
3.  The Qt 6 runtime libraries are present or statically linked.

<a name="system-requirements"/>

# 3 System Requirements

<a name="functional-requirements"/>

## 3.1 Functional Requirements

<a name="core-functions"/>

### 3.1.1 Core Functions

<a name="nickname-mgmt"/>

#### 3.1.1.1 Nickname Management
**Description.** The user must be able to identify themselves in the chat.
| Function | Requirements |
| :--- | :--- |
| Input | User enters text in the `nicknameLineEdit`. |
| Storage | Clicking "Save" writes the name to `nickname.txt` in the standard AppData location. |
| Loading | On app startup, the name is automatically read from the file. |
| Validation | Empty nicknames are not allowed for sending messages. |

<a name="server-creation"/>

#### 3.1.1.2 Server Creation (Hosting)
**Description.** A user initiates a chat session.
| Function | Requirements |
| :--- | :--- |
| Start Server | Clicking "Create Dialog" initializes a `QTcpServer` listening on the specified port. |
| IP Display | The app detects and displays the machine's LAN IPv4 address for others to see. |
| Client Handling | The server accepts multiple incoming TCP connections and maintains a list of sockets. |
| Broadcast | Messages received from one client are forwarded to all other connected clients. |

<a name="client-connection"/>

#### 3.1.1.3 Client Connection (Joining)
**Description.** A user joins an existing chat session.
| Function | Requirements |
| :--- | :--- |
| Connection | Clicking "Connect" initializes a `QTcpSocket` and attempts to connect to the specified IP and Port. |
| Status Feedback | The UI updates to show connection status (buttons change state, system message appears). |
| Disconnection | Users can safely disconnect, alerting the server. |

<a name="messaging"/>

#### 3.1.1.4 Messaging
**Description.** Sending and receiving text data.
| Function | Requirements |
| :--- | :--- |
| Encoding | Nicknames and Messages are Base64 encoded before transmission to handle special characters. |
| Packet Format | Data is sent as `timestamp:base64_name:base64_msg\n`. |
| Display | Incoming messages are decoded and displayed in the `chatBrowser` with timestamps and color coding (Blue for self, White for others). |

<a name="non-functional-requirements"/>

## 3.2 Non-Functional Requirements

<a name="quality-attributes"/>

### 3.2.1 Quality Attributes
1.  **Usability:** The interface uses a dark theme (via QSS) to reduce eye strain.
2.  **Reliability:** TCP ensures message delivery order. Broken connections are detected via socket state changes.
3.  **Performance:** Messages should appear instantly (< 50ms latency on standard Wi-Fi).

<a name="system-constraints"/>

### 3.2.2 System Constraints
1.  **Port Availability:** The default port 12345 must be free on the host machine.
2.  **OS Compatibility:** Windows, Linux, or macOS (wherever Qt 6 is supported).