#include "ui.h"
#include <stdlib.h>
#include <pthread.h>

// Global variables for the chat window and input window
WINDOW *chat_win, *input_win;

// Mutex to protect screen updates
pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Initializes the user interface (UI) for the chat.
 * 
 * This function initializes the `ncurses` library, sets up key configurations, 
 * and creates two windows: one for displaying chat messages and one for user 
 * input.
 */
void init_ui(void) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    
    int height = LINES - 3;
    int width = COLS;

    chat_win = newwin(height, width, 0, 0);
    input_win = newwin(3, width, height, 0);

    draw_border(input_win);
    scrollok(chat_win, TRUE);

    wrefresh(chat_win);
    wrefresh(input_win);
}

/**
 * @brief Cleans up the user interface and closes the ncurses library.
 * 
 * This function is called to clean up after the program ends, restoring the 
 * terminal to its normal state and freeing any resources used by ncurses.
 */
void cleanup_ui(void) {
    endwin();
}

/**
 * @brief Draws a border around a specified window.
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
 * @brief Clears the input line in the input window.
 * 
 * This function moves the cursor to the beginning of the input line, clears 
 * it, redraws the border, and then refreshes the window to update the display.
 */
void clear_input_line(void) {
    wmove(input_win, 1, 0);
    wclrtoeol(input_win);
    draw_border(input_win);
    wrefresh(input_win);
}

/**
 * @brief Displays a message in the chat window.
 * 
 * This function prints a formatted message in the chat window. It is protected 
 * by a mutex to ensure that only one thread can update the window at a time, 
 * avoiding potential race conditions.
 * 
 * @param username The username (or IP address) of the sender.
 * @param message The message to display in the chat window.
 */
void display_message(const char *username, const char *message) {
    pthread_mutex_lock(&screen_mutex);
    wprintw(chat_win, "%s ~ %s\n", username, message);
    wrefresh(chat_win);
    pthread_mutex_unlock(&screen_mutex);
}

