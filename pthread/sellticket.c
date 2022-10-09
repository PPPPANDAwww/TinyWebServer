/*
使用多线程实现买票的案例
有3个窗口，一共是100张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>  

int tickets = 100;

void * sellticket(void * arg){
    //卖票
    while (tickets > 0){
        usleep(3000);
        printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
        tickets--;
    }
    return NULL;
}

int main(){
    //创建3个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    //回收子线程资源,阻塞(分离后不能join)
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    //退出主线程
    pthread_exit(NULL);

    return 0;
}