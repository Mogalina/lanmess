#include "network.h"
#include "ui.h"
#include "chat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Retrieves the local IP address of the machine.
 * 
 * This function scans the available network interfaces and fetches the 
 * local IP address (ignoring the loopback interface). 
 *
 * @param local_ip A buffer to store the local IP addres.
 */
void get_local_ip(char *local_ip) {
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            if (strcmp(ifa->ifa_name, "lo") != 0) {
                inet_ntop(AF_INET, &sa->sin_addr, local_ip, INET_ADDRSTRLEN);
                break;
            }
        }
    }
    freeifaddrs(ifaddr);
}

/**
 * @brief Initializes a socket for UDP communication.
 * 
 * This function creates a UDP socket. If `broadcast` is true, it enables 
 * broadcast functionality for the socket.
 * 
 * @param broadcast A flag indicating whether the socket should support 
 * broadcasting.
 * @return The file descriptor of the created socket.
 */
int init_socket(int broadcast) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (broadcast) {
        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    }

    return sockfd;
}

/**
 * @brief Receives messages in a separate thread.
 * 
 * This function binds a UDP socket to listen for incoming messages on the 
 * specified port. 
 * 
 * @param arg Unused argument (required for pthread compatibility).
 * @return NULL.
 */
void *receive_messages(void *arg) {
    int sockfd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(addr);

    // Initialize a UDP socket with broadcasting enabled
    sockfd = init_socket(1);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Loop to continuously receive messages
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes_received = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0, 
                                    (struct sockaddr *)&addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            display_message(user_name, buffer);
        }
    }

    // Close the socket when done
    close(sockfd);

    return NULL;
}

/**
 * @brief Sends messages in the main thread.
 * 
 * This function handles sending messages to a broadcast address. It interacts 
 * with the user via the input window and sends the message over the UDP socket.
 */
void send_messages(void) {
    int sockfd;
    struct sockaddr_in addr;
    char message[BUF_SIZE];

    // Initialize a UDP socket with broadcasting enabled
    sockfd = init_socket(1);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    clear_input_line();

    // Main loop for sending messages
    while (1) {
        memset(message, 0, BUF_SIZE);
        
        pthread_mutex_lock(&screen_mutex);
        echo();
        curs_set(1);
        wmove(input_win, 1, 1);
        wrefresh(input_win);
        pthread_mutex_unlock(&screen_mutex);

        wgetnstr(input_win, message, BUF_SIZE - 1);
        if (strlen(message) > 0) {
            sendto(sockfd, message, strlen(message), 0, 
                   (struct sockaddr *)&addr, sizeof(addr));
            pthread_mutex_lock(&screen_mutex);
            clear_input_line();
            pthread_mutex_unlock(&screen_mutex);
        }
    }

    // Close the socket when done
    close(sockfd);
}

