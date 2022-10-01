#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(){
        //创建子进程
        pid_t pid = fork();
        int num = 0;
        //创建成功，子进程返回0， 父进程返回子进程的ID

        if(pid > 0){
            printf("I am parent process, pid : %d, ppid: %d\n", getpid(), getppid());
        }else if(pid == 0){
		    printf("I am child process, pid : %d, ppid: %d\n", getpid(), getppid());
            sleep(1);
	    }

        for (int i = 0; i < 4;i++){
            printf("i = %d, pid= %d\n", i, getpid());
        }

        return 0;
}