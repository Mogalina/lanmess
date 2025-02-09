#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <ifaddrs.h>

#define BROADCAST_IP "255.255.255.255"
#define PORT 12345
#define BUF_SIZE 1024

void get_local_ip(char *local_ip);
void *receive_messages(void *arg);
void send_messages(void);
int init_socket(int broadcast);

#endif

