#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <strings.h>
//响应头
#define H200NONKEEP "HTTP/1.1 200 OK\r\nConnection: close\r\nServer: wjhss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"
#define H200KEEP "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nServer: wjhiss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"
#define H405NONKEEP "HTTP/1.1 405 Request not allow\r\nConnection: close\r\nServer: wjhss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"
#define H405KEEP "HTTP/1.1 405 Request not allow\r\nConnection: Keep-Alive\r\nServer: wjhss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"
#define H404NONKEEP "HTTP/1.1 404 Not Found\r\nConnection: close\r\nServer: wjhss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"
#define H404KEEP "HTTP/1.1 404 Not Found\r\nConnection: Keep-Alive\r\nServer: wjhss\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"


typedef struct http_request
{
    int fd;
    int alive;
    char method[32];
    char url[1024];
}http_request_t;
//解析请求头
void http_header_get(http_request_t *ptr,char *header);
//获取请求
void http_recv(http_request_t *ptr,int epfd);
//响应
int http_send(http_request_t *ptr,int epfd);
void write_405(http_request_t *http_ptr);
void write_404(http_request_t *http_ptr);
//写文件
int send_data(http_request_t *ptr,char *filename);
//判断是否长连接
void judge_alive(http_request_t *ptr,int epfd);
#endif

