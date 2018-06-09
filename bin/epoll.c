#include "epoll.h"
#include "http_request.h"
//添加监听事件
void epoll_add(int epfd,http_request_t *ptr,int status)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.ptr=(void *)ptr;
    ev.events=status;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,ptr->fd,&ev)==-1)
    {
        perror("epoll ctl_add fail");
        return ;
    }
   // printf("epoll: %d  %d\n",epfd,ptr->fd);
}
//更改
void epoll_mod(int epfd,http_request_t *ptr,int status)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.ptr=ptr;
    ev.events=status;
    if(epoll_ctl(epfd,EPOLL_CTL_MOD,ptr->fd,&ev)==-1)
    {
        perror("epoll ctl_mod fail");
        return ;
    }
}
//删除
void epoll_del(int epfd,http_request_t *ptr,int status)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.ptr=ptr;
    ev.events=status;
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,ptr->fd,&ev)==-1)
    {
        perror("epoll ctl_del fail");
        return ;
    }
    free(ptr);
}

