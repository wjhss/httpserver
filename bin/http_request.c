#include "http_request.h"
#include "http.h"
#include "epoll.h"
#include "config.h"

extern http_conf_t conf;
void http_recv(http_request_t *http_ptr,int epfd)
{
    char header[10240];
    size_t n;
    int alive = 0;
    int socked=http_ptr->fd;
    n=recv(socked,header,10240,0);
    if(n<0)
    {
        perror("http_recv fial");
        free(http_ptr);
        close(socked);
    }
    else if(n==0)
    {
        //perror("connect close");
        // free(http_ptr);
        // close(socked);
        epoll_del(epfd,http_ptr,0);
    }
    else
    {
        http_header_get(http_ptr,header);
        epoll_mod(epfd,http_ptr,EPOLLOUT);
    }
}
int http_send(http_request_t *http_ptr,int epfd)
{
    int url_len,i,ret=0;
    char path[128];
    if(strcasecmp(http_ptr->method,"GET")!=0)
    {
        write_405(http_ptr);
        judge_alive(http_ptr,epfd);
        return -1;
    }
    url_len=strlen(http_ptr->url);
    for(i=1;i<url_len;i++)
        if(http_ptr->url[i]=='.'&&http_ptr->url[i-1]=='.')
        {
            write_405(http_ptr);
            judge_alive(http_ptr,epfd);
            return -1;
        }
    strcpy(path,conf.docroot);
    strcat(path,http_ptr->url);
    if(path[strlen(path)-1]=='/')
        strcat(path,"index.html");
    ret=send_data(http_ptr,path);
    judge_alive(http_ptr,epfd);
    return ret;
}
void http_header_get(http_request_t *http_ptr,char *header)
{
    size_t i,j;
    i=j=0;
    while(header[i]!=' ')
    {
        http_ptr->method[i]=header[i];
        i++;
    }
    http_ptr->method[i]='\0';
    while(header[i]==' ')i++;
    j=0;
    while(header[i]!=' ')
    {
        http_ptr->url[j]=header[i];
        j++,i++;
    }
    http_ptr->url[j]='\0';
    if(strcasestr(header,"keep-alive")!=NULL)
        http_ptr->alive=1;
    else
        http_ptr->alive=0;
}
void write_405(http_request_t *http_ptr)
{
    char data[]="<html> <h1>405</h1></html>\r\n";
    char header[256];
    if(http_ptr->alive)
        sprintf(header,H405KEEP,strlen(data));
    else
        sprintf(header,H405NONKEEP,sizeof(data));
    if(write(http_ptr->fd,header,strlen(header))<0)
        perror("write_405 header fail");
    if(write(http_ptr->fd,data,strlen(data))<0)
        perror("write_405 data fail");
}
int send_data(http_request_t *http_ptr,char *filename)
{
    struct stat st;
    char header[256];
    if(stat(filename,&st)==-1||S_ISDIR(st.st_mode))
    {
        write_404(http_ptr);
        return -1;
    }
    if(http_ptr->alive)
        sprintf(header,H200KEEP,st.st_size);
    else
        sprintf(header,H200NONKEEP,st.st_size);
    int fd=open(filename,O_RDONLY);
    if(fd==-1)
    {
        perror("open file fail");
        return -1;
    }
    if(write(http_ptr->fd,header,strlen(header))<0)
    {
        perror("send_data header fail");
        return -1;
    }

    if(sendfile(http_ptr->fd,fd,NULL,st.st_size)<0)
        perror("sendfile fial");
    close(fd);
    return 0;
}
void write_404(http_request_t *http_ptr)
{
    char data[]="<html> <h1>404</h1></html>\r\n";
    char header[256];
    if(http_ptr->alive)
        sprintf(header,H404KEEP,strlen(data));
    else
        sprintf(header,H404NONKEEP,sizeof(data));
    if(write(http_ptr->fd,header,strlen(header))<0)
        perror("write_404 header fail");
    if(write(http_ptr->fd,data,strlen(data))<0)
        perror("write_404 data fail");
}
void judge_alive(http_request_t *http_ptr,int epfd)
{
    if(http_ptr->alive)
    {
        epoll_mod(epfd,http_ptr,EPOLLIN);
    }
    else
    {
        epoll_del(epfd,http_ptr,EPOLLOUT);
        close(http_ptr->fd);
        free(http_ptr);
    }
}

