#!/bin/bash

INSTALL_DIR="/usr/local/bin"
PROGRAM_NAME="lanmess"

# Remove program file
remove_program() {
    if [ -f "$INSTALL_DIR/$PROGRAM_NAME" ]; then
        echo "Removing program..."
        if [ ! -w "$INSTALL_DIR" ]; then
            sudo rm "$INSTALL_DIR/$PROGRAM_NAME"
        else
            rm "$INSTALL_DIR/$PROGRAM_NAME"
        fi
    else
        echo "Program not found in $INSTALL_DIR"
    fi
}

# Remove alias from shell configuration
remove_alias() {
    SHELL_RC=""
    if [ -f "$HOME/.zshrc" ]; then
        SHELL_RC="$HOME/.zshrc"
    elif [ -f "$HOME/.bashrc" ]; then
        SHELL_RC="$HOME/.bashrc"
    elif [ -f "$HOME/.bash_profile" ]; then
        SHELL_RC="$HOME/.bash_profile"
    fi
    
    if [ -n "$SHELL_RC" ]; then
        echo "Removing alias from $SHELL_RC"
        sed -i.bak '/# 'Local Area Network Messenger' alias/d' "$SHELL_RC"
        sed -i.bak '/alias lanmess=/d' "$SHELL_RC"
        rm -f "${SHELL_RC}.bak"
    fi
}

# Main uninstallation process
main() {
    echo "Uninstalling 'Local Area Network Messenger'..."
    remove_program
    remove_alias
    echo "Uninstallation complete! Please restart your terminal or run 'source $SHELL_RC'"
}

main

