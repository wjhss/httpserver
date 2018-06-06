#ifndef _THREADD_H_
#define _THREAD_H_
#include "http_request.h"
#include "epoll.h"
#include "config.h"
void * thread_work(void *arg);

#endif
