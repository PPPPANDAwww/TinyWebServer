#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int value = 30;

void * callback(void * arg){
    printf("chilf thread id : %ld\n", pthread_self());
    for (int i = 0; i < 5; i++){
        printf("child:%d\n", i);
    }
    return NULL;
}

int main(){
    //创建一个线程属性变量
    pthread_attr_t attr;

    //初始化属性变量
    pthread_attr_init(&attr);

    //设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //设置线程分离

    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }
    
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread stack size :%ld\n", size);

    for (int i = 0; i < 5; i++){
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread_id:%ld\n", tid, pthread_self());

    //释放线程属性资源
    pthread_attr_destroy(&attr);
    //让主线程退出，不会影响其他正常运行的线程
    pthread_exit(NULL);

    return 0;   

}
