# Testing Documentation for IntranetChat

This directory contains all testing-related documentation for the IntranetChat project. Since the project relies on visual GUI interaction and network connectivity, the primary testing method is **Manual System Testing**.

## Documents Overview

### 📋 [TEST_PLAN.md](./TEST_PLAN.md)
Comprehensive test plan document that outlines:
- Test objectives and scope.
- Test strategy and approach.
- Hardware and software requirements.
- Risk assessment and mitigation.

### 📊 [TEST_RESULTS.md](./TEST_RESULTS.md)
Detailed logs of the executed test cases:
- List of checked functions (Connection, Messaging, Storage).
- Pass/Fail status for each case.
- Notes on observed behavior.

### 📝 [TESTING_SUMMARY.md](./TESTING_SUMMARY.md)
High-level executive summary:
- Key findings and statistics.
- Identified limitations (e.g., Firewall issues).
- Recommendations for future improvements.

## How to Perform Manual Testing

To verify the application functionality yourself, follow this **Quick Start Guide**:

### 1. Setup Environment
1.  Build the project using CMake/Qt Creator.
2.  Locate the executable file (`IntranetChat.exe` or similar).
3.  Ensure you have permission to allow network access (check Firewall settings).

### 2. Run Test Scenarios

**Scenario A: Loopback Test (Single PC)**
1.  Launch **Instance #1** of the application.
    *   Set Nickname: `Host`.
    *   Select **Server** mode -> Click **Create**.
    *   *Note the displayed IP.*
2.  Launch **Instance #2** of the application.
    *   Set Nickname: `Guest`.
    *   Select **Client** mode.
    *   Enter IP: `127.0.0.1` (Localhost).
    *   Click **Connect**.
3.  **Verify:**
    *   Both windows should show connection status.
    *   Send a message from `Host`. Does it appear on `Guest`?
    *   Send a message from `Guest`. Does it appear on `Host`?

**Scenario B: LAN Test (Two PCs)**
1.  Run **Server** on PC 1.
2.  Run **Client** on PC 2 connected to the same Wi-Fi.
3.  Enter PC 1's IP on PC 2.
4.  **Verify:** Connection is established and messages are exchanged instantaneously.

## Troubleshooting
*   **"Connection Refused":** Ensure the Server is actually running and you are using the correct Port.
*   **"Host Not Found":** Check if Windows Firewall is blocking the application on the Server PC. Allow the app in settings.

---

**Status:** Testing Complete
**Last Updated:** 2025-10-27