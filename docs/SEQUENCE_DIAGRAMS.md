# IntranetChat - Sequence Diagrams

## Overview
These diagrams visualize the interaction between the User, the Main Window Interface, and the system components (Network, Storage) over time for each Use Case.

## UC1: Save Nickname
This sequence shows how the application persists user settings to the local file system.

![UC1 Sequence](/docs/schema/sequence_uc1_nick.png)

## UC2: Create Server
The process of initializing the `QTcpServer` and querying the operating system for the local network address.

![UC2 Sequence](/docs/schema/sequence_uc2_server.png)

## UC3: Connect to Server
The TCP handshake process initiated by the Client to join a chat session.

![UC3 Sequence](/docs/schema/sequence_uc3_connect.png)

## UC4: Send Message
How user input is transformed into a network packet using Base64 encoding and transmitted via the socket.

![UC4 Sequence](/docs/schema/sequence_uc4_send.png)

## UC5: Receive Message
The asynchronous handling of incoming data signals (`readyRead`), buffer processing, decoding, and UI update.

![UC5 Sequence](/docs/schema/sequence_uc5_receive.png)