#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <pthread.h> 

extern WINDOW *chat_win, *input_win;
extern pthread_mutex_t screen_mutex;

void init_ui(void);
void cleanup_ui(void);
void draw_border(WINDOW *win);
void clear_input_line(void);
void display_message(const char *username, const char *message);

#endif

