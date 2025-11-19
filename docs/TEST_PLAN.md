# Test Plan for IntranetChat

## Document Information
- **Project:** IntranetChat
- **Version:** 1.0
- **Status:** Draft

## Table of Contents
1. [Introduction](#introduction)
2. [Test Objectives](#test-objectives)
3. [Test Scope](#test-scope)
4. [Test Strategy](#test-strategy)
5. [Test Environment](#test-environment)
6. [Test Schedule](#test-schedule)
7. [Risk Assessment](#risk-assessment)

## 1. Introduction

### 1.1 Purpose
The purpose of this Test Plan is to define the testing strategy for "IntranetChat", a C++/Qt based local network messenger. This document ensures that all functional requirements (messaging, connection handling) and non-functional requirements (performance, usability) are met before the final release.

### 1.2 Scope
This plan covers the testing of:
- The Desktop Client Interface (Qt Widgets).
- The Networking Logic (TCP Client/Server).
- Data persistence (Nickname storage).
- Stability under standard LAN conditions.

## 2. Test Objectives

### 2.1 Primary Objectives
- Verify that a User can successfully host a chat server.
- Verify that multiple Clients can connect to the Host.
- Ensure messages are delivered correctly to all participants (Broadcast).
- Confirm that special characters (Base64 encoding) do not break the protocol.
- Validate that the application handles disconnections gracefully without crashing.

## 3. Test Scope

### 3.1 In Scope
- **Functional Testing:**
    - Nickname saving/loading.
    - Server startup and port listening.
    - Client connection logic (valid/invalid IP).
    - Message transmission (text, symbols, empty strings).
- **UI/UX Testing:**
    - Dark mode rendering.
    - Responsiveness of buttons.
    - Text wrapping in chat window.

### 3.2 Out of Scope
- Testing on mobile platforms (Android/iOS) – Desktop only.
- Testing over the Internet (WAN) – LAN only.
- Encryption/Security penetration testing (beyond basic Base64 encoding).

## 4. Test Strategy

### 4.1 Approach
Given the nature of the project (Desktop GUI), the primary testing method will be **Manual System Testing**.
Testers will launch multiple instances of the application on the same machine (localhost) or different machines in the same Wi-Fi network to simulate real usage.

### 4.2 Test Levels
1.  **Unit Testing:** Verification of individual functions (e.g., `getLocalIp`, Base64 encoding helpers).
2.  **Integration Testing:** Verifying that `MainWindow` correctly interacts with `QTcpSocket`.
3.  **System Testing:** End-to-end testing of a complete chat session.

## 5. Test Environment

### 5.1 Hardware
- **PC 1 (Host):** Windows/Linux/macOS device to run the Server.
- **PC 2 (Client):** Second device for connection testing (optional, can use 2 windows on one PC).

### 5.2 Software
- **OS:** Windows 10/11, Ubuntu 20.04+, or macOS.
- **Network:** Local Wi-Fi or Ethernet connection.
- **Tools:**
    - IntranetChat Executable.
    - (Optional) Wireshark for packet inspection.

## 6. Test Schedule

| Phase | Activity | Duration |
| :--- | :--- | :--- |
| 1 | Unit Testing & Debugging | 2 Days |
| 2 | Functional Testing (Localhost) | 1 Day |
| 3 | Network Testing (LAN) | 1 Day |
| 4 | Bug Fixing | 1 Day |
| 5 | Final Release | - |

## 7. Risk Assessment

### 7.1 Risks
- **Firewall Blocking:** Windows Firewall might block the incoming connection on port 12345.
- **Dynamic IP:** If the Host's IP changes, Clients will disconnect.
- **Encoding Errors:** Non-Latin characters (Cyrillic/Emoji) might not display correctly if Base64 is flawed.

### 7.2 Mitigation
- Add instructions to allow the app in Firewall settings.
- Display the current IP prominently in the UI.
- Test specifically with Cyrillic strings during the Functional phase.