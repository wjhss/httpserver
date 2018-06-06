#include "config.h"

void http_conf_init(http_conf_t *conf)
{
    FILE *fp = NULL;
    char line[128],_name[128],_value[128];
    if((fp=fopen(CONFIG,"r")) == NULL)
    {
        perror("fail to open config file\n\n");
        return ;
    }
    while(fgets(line,128,fp)!=NULL)
    {
        sscanf(line,"%s %s",_name,_value);
        if(strcmp(_name,"http_port:")==0)
            conf->http_port=atoi(_value);
        if(strcmp(_name,"thread_num:")==0)
            conf->thread_num=atoi(_value);
        if(strcmp(_name,"listen_num:")==0)
            conf->listen_num=atoi(_value);
        if(strcmp(_name,"file_dir:")==0)
            strcpy(conf->docroot,_value);
        if(strcmp(_name,"events_list:")==0)
            conf->evlist_num=atoi(_value);
    }
    fclose(fp);
}
