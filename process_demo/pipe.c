//int pipe(int pipefd[2], int flags);
//功能：创建一个匿名管道，用来进行进程间通信
//参数：int pipefd[2]这个数组是一个传出参数
//      pipefd[0] 对应的是管道的读端
//      pipefd[1] 对应的是管道的写端
//返回值：成功返回0， 失败返回-1
//管道默认是阻塞的：如果管道中没有数据，read阻塞，如果管道满了，write阻塞
//注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(){
    //在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1){
        perror("pipe");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    char buf[1024] = {0};
    if(pid > 0){
        //父进程
        //从管道读取端读取数据
        int num = 5;
        while(num){
            int len = read(pipefd[0],buf, sizeof(buf));
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            
            //向管道中写入数据
            char * str  = "hello!I am parent. ";
            write(pipefd[1], str, strlen(str));
            num--;            
        }
        
        
    }else if(pid == 0){
        //子进程
        //从管道中写入数据
        int num = 5;
        printf("I am child process, pid : %d\n", getpid());
        while(num){
            char * str  = "hello I am child. ";
            write(pipefd[1], str, strlen(str));
            //sleep(1);

            int len = read(pipefd[0], buf, sizeof(buf));
            printf("child recv : %s, pid : %d\n", buf, getpid());         
            num--;
        }

    }

    return 0;
}