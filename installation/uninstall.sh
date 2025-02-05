#!/bin/bash

# Check if using Homebrew or need root
if [ "$EUID" -eq 0 ] && [ -x "$(command -v brew)" ]; then 
    echo "Please don't run as root when using Homebrew"
    exit 1
elif [ "$EUID" -ne 0 ] && ! [ -x "$(command -v brew)" ]; then
    echo "Please run as root (use sudo)"
    exit 1
fi

# Remove program
if [ -x "$(command -v brew)" ]; then
    rm -rf ~/Applications/lanmess
    
    # Remove from shell config
    if [ -f "$HOME/.zshrc" ]; then
        sed -i '' '/alias lanmess=/d' "$HOME/.zshrc"
    fi
    if [ -f "$HOME/.bash_profile" ]; then
        sed -i '' '/alias lanmess=/d' "$HOME/.bash_profile"
    fi
else
    # Remove from Linux
    rm -f /usr/local/bin/lanmess
    
    # Remove alias from all user's bashrc
    for USER_HOME in /home/*; do
        if [ -d "$USER_HOME" ]; then
            BASHRC="$USER_HOME/.bashrc"
            if [ -f "$BASHRC" ]; then
                sed -i '/alias lanmess=/d' "$BASHRC"
            fi
        fi
    done
fi

echo -e "\nUninstallation complete!"
if [ -x "$(command -v brew)" ]; then
    echo "Please restart your terminal or run: source ~/.zshrc (or ~/.bash_profile)"
else
    echo "Please restart your terminal or run: source ~/.bashrc"
fi

