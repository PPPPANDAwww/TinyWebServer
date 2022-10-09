#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * callback(void * arg){
    printf("child thread...\n");
    printf("arg value: %d\n", *(int *)arg); //先转换为int *，然后取值再加*
}

int main(){
    pthread_t tid;
    int num = 10;

    //创建一个子进程
    int ret = pthread_create(&tid, NULL, callback, (void *)&num); //取地址
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error : %s", errstr);
    }

    for (int i = 0; i < 5; i++){
        printf("%d\n", i);
    }
    sleep(1);

    return 0;
}