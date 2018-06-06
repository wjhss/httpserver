#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include "http_request.h"
//添加监听事件
void epoll_add(int epfd,http_request_t *ptr,int status);
//更改监听事件
void epoll_mod(int epfd,http_request_t *ptr,int status);
//删除监听事件
void epoll_del(int epfd,http_request_t *ptr,int status);

#endif

