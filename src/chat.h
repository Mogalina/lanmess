#ifndef CHAT_H
#define CHAT_H

#include <pthread.h>
#include "network.h"

extern char local_ip[INET_ADDRSTRLEN];
extern char user_name[BUF_SIZE];

void init_chat(const char *username);
void run_chat(void);
void cleanup_chat(void);

#endif

