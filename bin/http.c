#include "http.h"
#include "config.h"
#include "epoll.h"
#include "http_request.h"

http_conf_t conf;

int main()
{
    int client_sock=-1,server_sock=-1;
    int i,n;
    uint16_t server_port;
    int *epfds=NULL;
    pthread_t *threads=NULL;
    struct sockaddr_in client_addr;
    int client_len=sizeof(sizeof(struct sockaddr_in));
    http_conf_init(&conf);
    epfds=(int *)malloc(sizeof(int)*conf.thread_num);
    threads=(pthread_t *)malloc(sizeof(pthread_t)*conf.thread_num);

    if(epfds==NULL)
    {
        perror("epfd malloc fail\n\n");
        exit(EXIT_FAILURE);
    }
    server_port=conf.http_port;
    server_sock=startup(&server_port,conf.listen_num);

    for(i=0;i<conf.thread_num;i++)
    {
        if((epfds[i]=epoll_create1(0))<0)
        {
            perror("epoll create fail\n\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i=0;i<conf.thread_num;i++)
    {
        if(pthread_create(&threads[i],NULL,thread_work,(void *)(epfds+i))!=0)

        {
            perror("pthread create fail\n\n");
            exit(EXIT_FAILURE);
        }
    }
    while(1)
    {
        for(i=0;i<conf.thread_num;i++)
        {
            if((client_sock=accept(server_sock,(struct sockaddr *)&client_addr,&client_len))<0)
            {
                perror("accept fail\n\n");
                continue;
            }
            http_request_t *ptr=(http_request_t *)malloc(sizeof(http_request_t));
            ptr->fd=client_sock;
            set_sock_nonblock(ptr->fd);
            epoll_add(epfds[i],ptr,EPOLLIN);
            printf("ip:%s  port: %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        }
    }
    return 0;
}

int startup(uint16_t *server_port,int listen_num)
{
    struct sockaddr_in server_addr;
    int on=1;
    int server_sock=socket(AF_INET,SOCK_STREAM,0);
    if(server_sock==-1)
    {
        perror("socket faile");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(*server_port);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if((setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
        perror("setsockopt fail");
    setsockopt(server_sock,IPPROTO_TCP,TCP_NODELAY,&on,sizeof(on));
    if(bind(server_sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in))<0)
    {
        perror("buid fail");
        exit(EXIT_FAILURE);
    }

    if(listen(server_sock,listen_num)<0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
    return server_sock;

}

void set_sock_nonblock(int sock)
{
    int old_option=fcntl(sock,F_GETFL,0);
    if(old_option==-1)
    {
        perror("fcntl get old fail");
        return ;
    }
    if(fcntl(sock,F_SETFL,old_option | O_NONBLOCK)==-1)
    {
        perror("fcntl set nonblock fail");
        return ;
    }
}
