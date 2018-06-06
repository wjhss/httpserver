#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONFIG "../config/http.conf"
struct http_conf
{
    int http_port;//端口
    int thread_num;//线程池大小
    int listen_num;//最大监听大小
    char docroot[50];//文件资源目录
    int evlist_num;//events 大小
};
typedef struct http_conf http_conf_t;
void http_conf_init(http_conf_t *conf);

#endif
