#include<unistd.h>
#include <stdio.h>

int main(){
    int i = 0;
    int cnt = 20;
    for(;i < 2; ++i){
        if(fork() == 0){
            cnt --;
            printf("I am child process, pid : %d, ppid: %d, cnt: %d\n", getpid(), getppid(),cnt);
            printf("A\n");
        }else{
            cnt++;
            printf("I am parent process, pid : %d, ppid: %d, cnt: %d\n", getpid(), getppid(),cnt);
            printf("B\n");
        }
    }
    exit(0);
}