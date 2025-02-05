# lanmess

A lightweight command-line chat application for local network communication using UDP broadcast. Enables instant messaging within your local network without requiring a central server, making it perfect for quick team communications, local events, or classroom settings.

## Features

- Instant local network messaging
- No server required (uses UDP broadcast)
- Simple command-line interface
- Username support
- Cross-platform support (Linux, macOS)
- Auto-detection of network interface
- Clean interface using ncurses

## Prerequisites

- GCC compiler
- ncurses library (automatically installed by the installer)
- Network connection
- Root/sudo access for installation

## Installation

1. Extract the package:
   ```bash
   tar -xzf lanmess.tar.gz
   cd lanmess
   ```

2. Install the program:
   - For Linux:
     ```bash
     sudo ./install.sh
     ```
   - For macOS:
     ```bash
     ./install.sh
     ```

3. Restart your terminal or run:
   - For Linux:
     ```bash
     source ~/.bashrc
     ```
   - For macOS:
     ```bash
     source ~/.zshrc  # or ~/.bash_profile
     ```

## Usage

Start the program:
- Without username (uses IP address as identifier):
  ```bash
  lanmess
  ```
- With username:
  ```bash
  lanmess <username>
  ```

Navigation:
- Type your message and press `ENTER` to send
- Messages appear in the main window with sender identification
- Press `CTRL+C` to exit

## Uninstallation

To remove the program:
- For Linux:
  ```bash
  sudo ./uninstall.sh
  ```
- For macOS:
  ```bash
  ./uninstall.sh
  ```

## Troubleshooting

- If messages aren't sending, check your firewall settings for **UDP port 12345**
- If the interface looks broken, ensure your **terminal supports ncurses**
- For installation issues, ensure you have the **required permissions**

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

See also the list of [contributors](AUTHORS.md) who participated in this project.

## Security

For security-related issues, please read our [SECURITY.md](SECURITY.md) before reporting.

## Support

For help and support, please check [SUPPORT.md](SUPPORT.md).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for more details.
