#include "chat.h"
#include "network.h"
#include "ui.h"
#include <string.h>

// Global variables
char local_ip[INET_ADDRSTRLEN];  // Stores the local IP address
char user_name[BUF_SIZE];        // Stores the username
pthread_t recv_thread;           // Thread for receiving messages

/**
 * @brief Initializes the chat application.
 * 
 * This function sets the username for the chat. If the username is provided, 
 * it uses it, otherwise it fetches the local IP address and uses it as the 
 * username.
 * 
 * @param username A pointer to a string containing the username.
 */
void init_chat(const char *username) {
    if (username) {
        strncpy(user_name, username, BUF_SIZE - 1);
        user_name[BUF_SIZE - 1] = '\0';
    } else {
        get_local_ip(local_ip);
        strncpy(user_name, local_ip, BUF_SIZE - 1);
    }
    
    init_ui();
}

/**
 * @brief Runs the main chat loop.
 * 
 * This function creates a separate thread to continuously receive messages 
 * while allowing the main thread to handle sending messages.
 */
void run_chat(void) {
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    send_messages();
}

/**
 * @brief Cleans up resources before exiting the chat application.
 * 
 * This function ensures the receiving thread finishes and then cleans up 
 * the user interface resources.
 */
void cleanup_chat(void) {
    pthread_join(recv_thread, NULL);
    cleanup_ui();
}

