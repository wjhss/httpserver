#include "thread.h"

extern http_conf_t conf;

void *thread_work(void *arg)
{
    int epfd=*((int *)arg);
    struct epoll_event *evlist = NULL;
    evlist = (struct epoll_event *)malloc(sizeof(struct epoll_event)*conf.evlist_num);
    if(evlist==NULL)
    {
        perror("malloc evlist fail");
        exit(EXIT_FAILURE);
    }
    int n,i;
    while(1)
    {
        if((n=epoll_wait(epfd,evlist,conf.evlist_num-1,-1))<0)
        {
            //perror("epoll wait fail");
            continue;
        }
        for(i=0;i<n;i++)
        {
            http_request_t *http_ptr=(http_request_t *)evlist[i].data.ptr;
            if(evlist[i].events==EPOLLIN)
            {
                http_recv(http_ptr,epfd);
            }
            else if(evlist[i].events==EPOLLOUT)
            {
                http_send(http_ptr,epfd);
            }
        }
    }
    free(evlist);
}
