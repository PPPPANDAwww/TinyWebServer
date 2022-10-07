#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//过3秒以后，每隔2秒钟定时一次

int main(){

    struct itimerval new_value;

    //设置间隔值
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟的时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    
    int ret = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞
    printf("timer start");

    if (ret == -1){
        perror("setitimer");
        exit(0);
    }
    getchar();


    return 0;
}
