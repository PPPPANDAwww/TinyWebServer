/*
int kill(pid_t pid, int sig);
功能：给某个进程或进程组pid，发送某个信号sig
参数：pid:进程id，

sig：需要发送的信号编号或者宏值
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(){
    pid_t pid = fork();
    
    if (pid == 0){
        //子进程
        int i = 0;
        for (i = 0; i < 5; i++){
            printf("child process\n");
            sleep(1);
        }


    }else if(pid > 0){
        //父进程
        printf("parent process\n");
        sleep(2);
        printf("kill child process now!\n");
        kill(pid, SIGINT);

    }

    return 0;
}