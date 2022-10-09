/*
#include <pthread.h>

void pthread_exit(void *retval);

功能：终止一个线程，在哪个线程中调用，就表示终止哪个进程
参数：
    -retval：需要传递一个指针，作为一个返回值，可以在pthread_join()中获取
返回值：没有返回值

pthread_t pthread_self(void);
功能：获取当前线程的id

int pthread_equal(pthread_t t1, pthread_t t2)
    功能：比较两个线程ID是否相等
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void * callback(void * arg){
    printf("chilf thread id : %ld\n", pthread_self());
    return NULL;
}

int main(){
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
  
  
    //让主线程退出，不会影响其他正常运行的线程
    pthread_exit(NULL);


    return 0;   

}