#include<stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void create_pipe(char *pipe_name)
{
    if(access(pipe_name, R_OK|W_OK) != -1)
        return ;
    if(mkfifo(pipe_name,0644) < 0)    //建立一个命名管道
    {
        perror("mkfifo");
        exit(-1);
    }
}
void read_pipe(char *pipe_name,char *result) //读管道
{
    int fd;
	fd = open(pipe_name,O_RDONLY,0);
    if(fd < 0)
    {
        perror("open");
        exit(-1);
    }
   printf("start read pipe\n");
    if(fd != -1)
    {
        if(read(fd,result,1000) != -1)
        {
            return ;
        }
    }
    close(fd);
    return ;
}
void write_pipe(char *pipe_name,char *content) //写管道
{
    int fd;
    if((fd = open(pipe_name,O_WRONLY,0)) < 0)
    {
        perror("open");
        exit(-1);
    }
 printf("start write pipe\n");
    if(fd != -1)
    {
        if(write(fd,content,1000) == -1)
            return ;
    }
    close(fd);
    return ;
}

