# IntranetChat - Deployment Diagrams

## Overview
This document illustrates the physical deployment of the IntranetChat application within a Local Area Network (LAN).

## Network Topology

The system operates on a Star Topology where one machine acts as the central Server (Host), and all other machines connect to it as Clients.

![Deployment Diagram](/docs/schema/deployment_diagram.svg)

### Deployment Nodes:

1.  **Host Device (Server):**
    *   The machine that initiates the chat session.
    *   Must have an open inbound port (default: 12345).
    *   Broadcasts messages to all connected nodes.

2.  **Guest Devices (Clients):**
    *   Machines that connect to the Host via IP address.
    *   Do not communicate with each other directly; all traffic goes through the Host.

3.  **Communication Channel:**
    *   Standard Wi-Fi or Ethernet connection.
    *   Protocol: TCP/IP.
    *   No Internet access required.