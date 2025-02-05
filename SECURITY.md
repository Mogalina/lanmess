# 🔒 Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| 1.0.0   | 🟢        |
| < 1.0   | 🔴        |

## Reporting a Vulnerability

1. **Reporting Process**
   - Email security issues to: eric.moghioros000@gmail.com
   - Include `lanmess security vulnerability` in the subject line
   - Provide detailed information about the vulnerability
   - If possible, include steps to reproduce the issue

2. **What to Expect**
   - You will receive acknowledgment of your report within 48 hours
   - We will investigate and provide regular updates on the status
   - Once fixed, you will be notified and can verify the solution

3. **Guidelines**
   - Please do not disclose security vulnerabilities publicly
   - Do not test vulnerabilities on production systems
   - Provide sufficient time for us to address the issue before any disclosure

4. **Known Security Considerations**
   - LanMess uses UDP broadcast for communication
   - Messages are not encrypted by default
   - The program operates on port 12345
   - Local network traffic can be intercepted

5. **Scope**
   - Network communication security
   - User input validation
   - System resource usage
   - Installation script security
   - Configuration file permissions

## Security Best Practices for Users

1. Use `lanmess` **only** on **trusted** local networks
2. Keep your system and the application up to date
3. Use a **firewall** to control UDP broadcast traffic
4. Be cautious when **sharing sensitive information**
5. Monitor system resources while using the application

## Recent Updates

We are committed to maintaining the security of `lanmess`. Check this section for security-related updates and patches.

- 2025-02-05: Initial security policy established

Thank you for helping keep `lanmess` and its users safe!
