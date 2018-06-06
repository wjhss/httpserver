#ifndef _HTTP_H_
#define _HTTP_H_

#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "thread.h"

int startup(uint16_t *server_port,int listen_num);
void set_sock_nonblock(int sock);
#endif
