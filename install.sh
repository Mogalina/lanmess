#!/bin/bash

# Check if running as root (except brew)
if [ "$EUID" -eq 0 ] && [ -x "$(command -v brew)" ]; then 
    echo "Please don't run as root when using Homebrew"
    exit 1
elif [ "$EUID" -ne 0 ] && ! [ -x "$(command -v brew)" ]; then
    echo "Please run as root (use sudo)"
    exit 1
fi

# Detect package manager and install ncurses
if [ -x "$(command -v brew)" ]; then
    brew install ncurses
elif [ -x "$(command -v apt-get)" ]; then
    apt-get update
    apt-get install -y libncurses5-dev
elif [ -x "$(command -v yum)" ]; then
    yum install -y ncurses-devel
elif [ -x "$(command -v pacman)" ]; then
    pacman -Sy ncurses
else
    echo -e "\nCould not detect package manager. Please install ncurses manually."
    exit 1
fi

# Compile the program
gcc -o lanmess lanmess.c -lpthread -lncurses

# Create program directory and copy executable
if [ -x "$(command -v brew)" ]; then
    # For macOS
    mkdir -p ~/Applications/lanmess
    cp lanmess ~/Applications/lanmess/
    INSTALL_PATH="$HOME/Applications/lanmess/lanmess"
    chmod +x "$INSTALL_PATH"
else
    # For Linux
    mkdir -p /usr/local/bin
    cp lanmess /usr/local/bin/
    INSTALL_PATH="/usr/local/bin/lanmess"
    chmod +x "$INSTALL_PATH"
fi

# Add to shell config file
if [ -x "$(command -v brew)" ]; then
    # For macOS, check for both bash and zsh
    if [ -f "$HOME/.zshrc" ]; then
        RCFILE="$HOME/.zshrc"
    else
        RCFILE="$HOME/.bash_profile"
    fi
    
    if ! grep -q "alias lanmess=" "$RCFILE"; then
        echo "alias lanmess='$INSTALL_PATH'" >> "$RCFILE"
    fi
else
    # For Linux
    for USER_HOME in /home/*; do
        if [ -d "$USER_HOME" ]; then
            BASHRC="$USER_HOME/.bashrc"
            if [ -f "$BASHRC" ]; then
                if ! grep -q "alias lanmess=" "$BASHRC"; then
                    echo "alias lanmess='$INSTALL_PATH'" >> "$BASHRC"
                fi
            fi
        fi
    done
fi

echo -e "\nInstallation complete!"
if [ -x "$(command -v brew)" ]; then
    echo "Please restart your terminal or run: source $RCFILE"
else
    echo "Please restart your terminal or run: source ~/.bashrc"
fi
echo -e "\nUsage:"
echo "   lanmess               ~ Using LAN IP Address"
echo "   lanmess <username>    ~ Using custom username"

