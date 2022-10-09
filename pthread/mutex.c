#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int tickets = 1000;
//创建一个互斥量（不能放在main函数）
pthread_mutex_t mutex;

void * sellticket(void * arg){
    //卖票，临界区
    
    while (1){
        //加锁
        pthread_mutex_lock(&mutex);
        usleep(6000);
        if (tickets > 0){
            printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
            tickets--;
        }else{
            //解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        //解锁
        //pthread_mutex_unlock(&mutex);

    }


    return NULL;
}

int main(){
    //初始化mutex
    pthread_mutex_init(&mutex, NULL);

    //创建3个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    //回收子线程资源,阻塞(分离后不能join)
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    //退出主线程
    pthread_exit(NULL);

    return 0;
}