// Imports and configuration
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <ncurses.h>

#define BROADCAST_IP "255.255.255.255"  // Broadcast address for sending messages
#define PORT 12345                      // Port used for communication
#define BUF_SIZE 1024                   // Maximum size for message buffers

// Global variables for storing local IP, username, and ncurses windows
char local_ip[INET_ADDRSTRLEN];
char user_name[BUF_SIZE];
WINDOW *chat_win, *input_win;

// Mutex for synchronizing access to the screen
pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Function to get the local IP address (non-loopback).
 * Stores the result in the global variable `local_ip`.
 */
void get_local_ip() {
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
 * Draws a border around the given ncurses window.
 *
 * @param win The window to draw the border around.
 */
void draw_border(WINDOW *win) {
    int x, y;
    getmaxyx(win, y, x);
   
    mvwhline(win, 0, 0, '-', x); 
    mvwhline(win, y-1, 0, '-', x);
}

/**
 * Thread function for receiving messages.
 * Continuously listens for messages on the specified port and displays them in the chat window.
 */
void *receive_messages(void *arg) {
    int sockfd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcast
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    // Bind the socket to the specified port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Continuously receive and display messages
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes_received = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0, (struct sockaddr *) &addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            pthread_mutex_lock(&screen_mutex);
            wprintw(chat_win, "%s ~ %s\n", 
                strlen(user_name) > 0 ? user_name : inet_ntoa(addr.sin_addr), 
                buffer);    
            wrefresh(chat_win);
            pthread_mutex_unlock(&screen_mutex);
        }
    }

    close(sockfd);
    return NULL;
}

/**
 * Clears the input line and redraws the border in the input window.
 */
void clear_input_line() {
    wmove(input_win, 1, 0);
    wclrtoeol(input_win);
    draw_border(input_win);
    wrefresh(input_win);
}

/**
 * Function for sending messages.
 * Continuously accepts user input from the input window and sends it as a broadcast message.
 */
void send_messages() {
    int sockfd;
    struct sockaddr_in addr;
    char message[BUF_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }        

    // Enable broadcast
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    // Configure the broadcast address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    clear_input_line();

    // Continuously read user input and send messages
    while (1) {
        memset(message, 0, BUF_SIZE);
    
        pthread_mutex_lock(&screen_mutex);
        echo();
        curs_set(1);
        wmove(input_win, 1, 1);
        wrefresh(input_win);
        pthread_mutex_unlock(&screen_mutex);

        // Retrieve user input
        wgetnstr(input_win, message, BUF_SIZE - 1); 
        if (strlen(message) > 0) {
            sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr));
            pthread_mutex_lock(&screen_mutex);
            clear_input_line();
            pthread_mutex_unlock(&screen_mutex);
        }
    }

    close(sockfd);
}

/**
 * Main function.
 * Initializes ncurses, creates windows, starts the message receiving thread, and runs the 
 * message sending loop.
 */
int main(int argc, char *argv[]) {
    pthread_t recv_thread;

    // Check if username is provided
    if (argc > 1) {
        strncpy(user_name, argv[1], BUF_SIZE - 1);
        user_name[BUF_SIZE - 1] = '\0';
    } else {
        get_local_ip();
    }

    // Initialize ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    
    int height = LINES - 3;  // Chat window height
    int width = COLS;        // Chat window width

    // Create chat and input windows
    chat_win = newwin(height, width, 0, 0);
    input_win = newwin(3, width, height, 0);

    // Draw border around the input window
    draw_border(input_win);

    // Enable scrolling in chat window
    scrollok(chat_win, TRUE); 

    // Refresh windows
    wrefresh(chat_win);
    wrefresh(input_win);

    // Create a thread for receiving messages
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    send_messages();

    // Wait for the receive thread to finish
    pthread_join(recv_thread, NULL);

    // End ncurses mode
    endwin();
    return 0;
}

