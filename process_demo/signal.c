#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//过3秒以后，每隔2秒钟定时一次

void myalarm(int num){
    printf("捕捉到了信号的编号是: %d\n", num);
    printf("xxxxxx\n");

}

int main(){
    //注册信号捕捉
    //void (*sighandler_t)(int); 函数指针，int类型的参数表示捕捉到的信号的值
    signal(SIGALRM, myalarm);

    struct itimerval new_value;

    //设置间隔值
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟的时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    int ret = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞
    printf("timer start\n");

    if (ret == -1){
        perror("setitimer");
        exit(0);
    }
    getchar();


    return 0;
}
