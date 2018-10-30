#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<stdlib.h>

static int order_num = 7;
const char *use_orders[6] = {"1","2","3","4","5","6"}; //主键盘输入的命令   
const char *linux_orders[6] = {"ls","cat","pwd","rm","move","gedit"};
//对应的linux的命令
char *order_change(char *use_order)
{
    int i;
    char *temp;
    char *order_pre;
    char *order_para;
    if((temp = (char*)malloc(20*sizeof(char))) ==0)
        return 0;
    if((order_pre = (char*)malloc(20*sizeof(char))) ==0)
        return 0;
    if((order_para = (char*)malloc(20*sizeof(char))) ==0)
        return 0;
          sscanf(use_order,"%s%[^'\n']",order_pre,order_para );//键盘输入所要执行的命令
    printf("order_pre   :%s\n",order_pre);
    printf("order_para   :%s\n",order_para);
    if(!strcmp(order_pre,"cd")&& strlen(order_para) == 0) // 
        return "pwd";
    else if(!strcmp(order_pre,"cd") && strlen(order_para) != 0)//
    {
        chdir("home");//在home目录下
    }
    for(i=0;i<order_num;i++)       //开始进行定义的键盘输入与linux中的命令相对应
    {
        if(strstr(order_pre,use_orders[i]) != 0)
        {
            strcpy(temp,linux_orders[i]);//strcpy（）函数调用 temp为后面地址的内容
            strcat(temp,order_para);   //strcat（）函数调用
            return temp;                //返回
        }
    }
    return "";
}

char *loadpath(char *pwd, char *filename)  //输出进行对文件操作的目录
{
    char path[1024];            
    strcpy(path,pwd);
    strcat(path,"/");
    strcat(path,filename);
    return path;
}
main()   //主函数
{
    pid_t pid;
    char *u_order;
    char *linux_order;
    if((u_order = (char*)malloc(20*sizeof(char))) == 0)
        return 0;
    if((linux_order = (char*)malloc(20*sizeof(char))) ==0)
        return 0;
    char result[1001];
    char pwd[1024];
    char pro_path[1024];
    getcwd(pro_path,1024);
    create_pipe(loadpath(pro_path,"lx_pipe")); //创建管道
    create_pipe(loadpath(pro_path,"syn_pipe"));
    create_pipe(loadpath(pro_path,"pwd_pipe"));
    signal(SIGCLD,SIG_IGN);
//输出每个命令所对应的序号值，方便用户参考
printf("1.ls\n");
	printf("2.cat\n");
	printf("3.pwd\n");
	printf("4.rm\n");
	printf("5.move\n");
	printf("6.gedit\n");

//线程的执行
    switch(pid=fork())
    {
        case -1:
            perror("fork");
            break;
        case 0:       //子进程  用来执行键盘输入对应的命令操作        
{
            if(execl(loadpath(pro_path,"background"),NULL)== -1)
//执行文件路径background
            {
                perror("execl");
                exit(0);
            }
            break;
        }
        default:        //父进程 其中要进行管道的读和消息的发送    
{
            while(1)
            {
                read_pipe(loadpath(pro_path,"pwd_pipe"),&pwd);
                sscanf(pwd,"%[^ ]",pwd);
                printf("%s:",pwd);
                gets(u_order);
                //

                if(strcmp(u_order,"exit") == 0)
                {
                    kill(pid,SIGKILL);   //进程结束
                    exit(0);
                }
                linux_order = order_change(u_order);
                write_pipe(loadpath(pro_path,"syn_pipe"),"Start Send");
                //  发送消息
                if(strlen(linux_order) == 0)
                {
              printf("Couldn't found the order,check your order\n");
                  continue;
                }
                send_msg(linux_order,pro_path);
                //  读取管道内容
                read_pipe(loadpath(pro_path,"lx_pipe"),&result);
                printf("%s\n",result);
            }
            break;
        }
    }
}

