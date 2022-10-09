/*
#include <pthread.h>

int pthread_detach(pthread_t thread);
    -功能：分离一个线程
    -参数:thread

*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int value = 30;

void * callback(void * arg){
    printf("chilf thread id : %ld\n", pthread_self());
    //sleep(3);
    //return NULL;
    pthread_exit((void *) &value); //return (void *) &value 线程退出时不要返回局部变量，无法传递
}

int main(){
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }

    for (int i = 0; i < 5; i++){
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread_id:%ld\n", tid, pthread_self());

    //设置子线程分离，子线程分离后，子线程结束时对应的资源就不需要主线程释放
    pthread_detach(tid);
    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }

    //让主线程退出，不会影响其他正常运行的线程
    pthread_exit(NULL);

    return 0;   

}