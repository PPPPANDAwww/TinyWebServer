/*
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
    -功能：和一个已经终止的线程进行连接，回收子线程的资源
    -特点：1、阻塞函数，直到有能回收的出现
         2、调用一次只能回收一个子线程
         3、一般在主线程中使用
    -参数：
        -thread：需要回收的子线程的id
        -retval：二级指针，接收子线程退出时的返回值
    -返回值：
        -成功：0
        -失败：非0， 返回错误号


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

    //主线程调用pthread_join回收子线程资源
    int * thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval); //第二个参数要求传递二级指针

    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }
    printf("子线程回收成功！\n");
    printf("exit data :%d\n", *thread_retval);
  
    //让主线程退出，不会影响其他正常运行的线程
    pthread_exit(NULL);

    return 0;   

}