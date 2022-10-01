#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(){
        //创建子进程
        pid_t pid = fork();
        int num = 0;
        //创建成功，子进程返回0， 父进程返回子进程的ID

        if(pid > 0){
                printf("pid : %d\n", pid);
                printf("I am parent process, pid : %d, ppid: %d\n", getpid(), getppid());
                printf("parent num : %d\n", num);
                num += 10;
                printf("parent num += 10: %d\n", num);
        }else if(pid == 0){
		printf("I am child process, pid : %d, ppid: %d\n", getpid(), getppid());
                printf("child num : %d\n", num);
                num += 10;
                printf("child num += 10: %d\n", num);
	}

        for (int i = 0; i < 4;i++){
                printf("i = %d, pid= %d\n", i, pid);
                sleep(1);
        }

        return 0;
}

