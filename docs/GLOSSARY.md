# IntranetChat - Glossary of Key Terms

**Base64**
A binary-to-text encoding scheme. IntranetChat uses it to encode user nicknames and message bodies before network transmission. This ensures that special characters (like colons or newlines) do not break the packet structure.

**Broadcast**
The process of sending a message from the Server to all currently connected Clients simultaneously. Implemented in the code via the `broadcastMessage` function, which iterates through the list of active sockets.

**Client**
An application mode where the software initiates a connection to a Server using a specific IP address and port. The Client sends messages to the server and displays incoming messages from other participants. Implemented using `QTcpSocket`.

**IP Address**
A unique numerical identifier for a device on a network. The application automatically detects the device's local IPv4 address in Server mode (via the `getLocalIp` function) so other users can connect to it.

**LAN (Local Area Network)**
A computer network that interconnects computers within a limited area. This is the target environment for IntranetChat, allowing communication within a single room or Wi-Fi network without Internet access.

**Nickname**
A unique user identifier displayed in the chat. It is stored locally in a `nickname.txt` file (in the system's AppData directory) and loads automatically upon application startup.

**Packet**
A unit of data transmitted between Client and Server. IntranetChat uses a text-based protocol with the format: `timestamp:base64_nickname:base64_message\n`.

**Port**
A number (1-65535) used to identify a specific service on a host. The application defaults to port `12345`. The Server listens on this port, and Clients connect to it.

**QSS (Qt Style Sheets)**
A mechanism for styling the user interface, similar to CSS in web development. Used in the application to implement the "Dark Mode" look (colors, borders, fonts) directly within the C++ code.

**Server**
An application mode where the software waits for incoming connections from other users. The Server manages the list of connected sockets and relays messages between them. Implemented using `QTcpServer`.

**Socket**
A software endpoint for sending or receiving data across a computer network. The project uses TCP sockets to ensure a reliable communication channel that guarantees message delivery order.

**Timestamp**
A numeric value (seconds since the Unix Epoch) added to every message to record when it was sent. It is used to display the correct time next to messages in the chat interface.