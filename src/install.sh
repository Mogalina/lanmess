#!/bin/bash

# Define installation paths
INSTALL_DIR="/usr/local/bin"
PROGRAM_NAME="lanmess"
SOURCE_FILES="main.c network.c ui.c chat.c network.h ui.h chat.h"
MAKEFILE="Makefile"

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required dependencies
check_dependencies() {
    echo "Checking dependencies..."
    
    # Check for gcc
    if ! command_exists gcc; then
        echo "Error: gcc is not installed"
        exit 1
    fi
    
    # Check for make
    if ! command_exists make; then
        echo "Error: make is not installed"
        exit 1
    fi
    
    # Check for ncurses
    if [ "$(uname)" == "Darwin" ]; then
        # macOS
        if ! pkg-config --exists ncurses; then
            echo "Error: ncurses is not installed. Install with: brew install ncurses"
            exit 1
        fi
    else
        # Linux
        if ! pkg-config --exists ncurses; then
            echo "Error: ncurses development files are not installed"
            echo "Install with: sudo apt-get install libncurses-dev (Ubuntu/Debian)"
            echo "or: sudo dnf install ncurses-devel (Fedora/RHEL)"
            exit 1
        fi
    fi
}

# Create temporary build directory
create_build_dir() {
    BUILD_DIR=$(mktemp -d)
    echo "Created temporary build directory: $BUILD_DIR"
    
    # Copy source files to build directory
    cp $SOURCE_FILES $MAKEFILE "$BUILD_DIR/"
}

# Compile the program
compile_program() {
    echo "Compiling program..."
    cd "$BUILD_DIR"
    make
    if [ $? -ne 0 ]; then
        echo "Compilation failed"
        exit 1
    fi
}

# Install the program
install_program() {
    echo "Installing program..."
    
    # Request sudo privileges
    if [ ! -w "$INSTALL_DIR" ]; then
        echo "Requesting administrator privileges to install to $INSTALL_DIR"
        sudo mv lanmess "$INSTALL_DIR/$PROGRAM_NAME"
    else
        mv lanmess "$INSTALL_DIR/$PROGRAM_NAME"
    fi
    
    # Add alias to appropriate shell configuration file
    SHELL_RC=""
    if [ -f "$HOME/.zshrc" ]; then
        SHELL_RC="$HOME/.zshrc"
    elif [ -f "$HOME/.bashrc" ]; then
        SHELL_RC="$HOME/.bashrc"
    elif [ -f "$HOME/.bash_profile" ]; then
        SHELL_RC="$HOME/.bash_profile"
    fi
    
    if [ -n "$SHELL_RC" ]; then
        echo "Adding alias to $SHELL_RC"
        echo "# 'Local Area Network Messenger' alias" >> "$SHELL_RC"
        echo "alias lanmess='$INSTALL_DIR/$PROGRAM_NAME'" >> "$SHELL_RC"
    else
        echo "Could not find shell configuration file. Please add the following line manually:"
        echo "alias lanmess='$INSTALL_DIR/$PROGRAM_NAME'"
    fi
}

# Cleanup
cleanup() {
    echo "Cleaning up..."
    rm -rf "$BUILD_DIR"
}

# Main installation process
main() {
    echo "Starting installation of 'Local Area Network Messenger'..."
    check_dependencies
    create_build_dir
    compile_program
    install_program
    cleanup
    echo "Installation complete! Please restart your terminal or run 'source $SHELL_RC'"
}

main

