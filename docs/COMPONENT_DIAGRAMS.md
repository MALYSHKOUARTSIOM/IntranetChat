# IntranetChat - Component Diagrams

## Overview
This document provides a high-level view of the software components that make up the IntranetChat application and their interactions with the external environment.

## Component Structure

The application is monolithic but logically divided into several modules responsible for specific tasks: UI rendering, Network communication, Data parsing, and Storage.

![Component Diagram](/docs/schema/component_diagram.svg)

### Components Description:

1.  **User Interface (MainWindow):**
    *   The central hub of the application. Handles user input, displays chat history, and manages application state (Server/Client modes).
2.  **Protocol Parser:**
    *   Responsible for formatting outgoing messages (adding timestamps, Base64 encoding) and parsing incoming data streams into readable text.
3.  **Network Manager:**
    *   Wraps `QTcpServer` and `QTcpSocket` logic. Manages connections, error handling, and raw byte transmission.
4.  **Data Storage:**
    *   Handles persistence of user settings (specifically the nickname) to the local file system.
5.  **Qt Framework:**
    *   The underlying library providing the event loop, widget set, and socket implementations.