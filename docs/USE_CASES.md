# IntranetChat - Use Case Analysis

## Actors
1.  **User (Host/Server):** The user who starts the chat session and acts as the central node.
2.  **User (Guest/Client):** A user who connects to an existing chat session initiated by a Host.

## Use Case Scenarios

### UC1: Save Nickname

**Actor:** User (Host or Guest)
**Precondition:** The application is launched.
**Flow of Events:**
1.  User navigates to the "Settings Panel".
2.  User enters a desired name in the `nicknameLineEdit`.
3.  User clicks the "Save" button.
4.  System validates that the input is not empty.
5.  System writes the nickname to `nickname.txt` in the AppData directory.
6.  System displays a status message "Nickname successfully saved!".
    **Postcondition:** The nickname is stored and will be used for subsequent messages.

### UC2: Create Server (Start Chat)

**Actor:** User (Host)
**Precondition:** Nickname is set. Port 12345 is free.
**Flow of Events:**
1.  User selects the "Server" radio button.
2.  System shows the server configuration pane.
3.  User clicks "Create Dialog".
4.  System initializes `QTcpServer` and starts listening on port 12345.
5.  System detects the local IP address and displays it in `ipAddressLabel`.
6.  System disables the "Create" button and enables "Shutdown Server".
7.  System logs "Server started..." in the chat window.
    **Postcondition:** The application is ready to accept incoming connections.

### UC3: Connect to Server (Join Chat)

**Actor:** User (Guest)
**Precondition:** Nickname is set. A Host is running on the network.
**Flow of Events:**
1.  User selects the "Client" radio button.
2.  User enters the Host's IP address in `serverIpLineEdit`.
3.  User enters the Port (default 12345).
4.  User clicks "Connect".
5.  System initializes `QTcpSocket` and attempts a handshake with the Host.
6.  **Success:**
    *   System logs "Connected to server!".
    *   UI switches to "Connected" state (Connect button hidden, Disconnect shown).
7.  **Failure:**
    *   System shows an error message box (e.g., "Host not found").
        **Postcondition:** A TCP connection is established between Guest and Host.

### UC4: Send Message

**Actor:** User (Host or Guest)
**Precondition:** Connection is established (Server is running or Client is connected).
**Flow of Events:**
1.  User types text into `messageLineEdit`.
2.  User presses "Enter" or clicks "Send".
3.  System captures the current Unix timestamp.
4.  System retrieves the stored Nickname.
5.  System encodes Nickname and Message using Base64.
6.  System constructs the packet: `timestamp:encoded_nick:encoded_msg\n`.
7.  **If Host:** System displays the message locally and broadcasts the packet to all connected Clients.
8.  **If Guest:** System sends the packet to the Server.
9.  System clears the input field.
    **Postcondition:** Message is sent to the network and displayed in the chat history.

### UC5: Receive Message

**Actor:** User (Host or Guest)
**Trigger:** Data arrives at the network socket (`readyRead` signal).
**Flow of Events:**
1.  System reads data from the socket into an internal buffer.
2.  System checks for the newline character `\n` (end of packet).
3.  System extracts the line and splits it by colons `:`.
4.  System decodes the Base64 parts (Nickname and Message).
5.  System formats the string: `[Time] Nickname: Message`.
6.  System appends the formatted string to the `chatBrowser`.
7.  **If Host:** System re-broadcasts the received packet to all *other* connected clients so they also see the message.
    **Postcondition:** The message is visible to the user.