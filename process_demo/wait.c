#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0) {
        printf("I'm child,my pid = %d, my parent = %d\n", getpid(), getppid());
        sleep(10);
        printf("---------------I'm going to die-------------------\n");
        // 特殊值，演示正常退出的返回值
        return 73;
    }
    else if (pid > 0) {
        // 父进程回收子进程 如果子进程未终止，父进程会阻塞在这个函数上
        // wpid = wait(NULL); // 不关心子进程结束原因
        wpid = wait(&status);
        if (wpid == -1) {
            perror("wait error\n");
            exit(1);
        }
        // 为真
        if (WIFEXITED(status)) {
            // 说明子进程正常终止
            printf("child exit with %d\n", WEXITSTATUS(status));
        }
        // 为真，说明子进程被信号终止
        if (WIFSIGNALED(status)) {
            printf("child is killed signal with %d\n",WTERMSIG(status));
        }
        printf("parent wait finish:%d\n", wpid);
    } else if (pid == -1) {
        perror("fork error\n");
        exit(1);
    }
 
    return 0;
}