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
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }
    //取消线程
    pthread_cancel(tid);

    for (int i = 0; i < 5; i++){
        printf("%d\n", i);
    }

    printf("tid : %ld, main thread_id:%ld\n", tid, pthread_self());

/*主线程调用pthread_join回收子线程资源
    int * thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval); //第二个参数要求传递二级指针

    if (ret != 0){
        char * errstr = strerror(ret);
        printf("error:%s", errstr);
    }
    printf("子线程回收成功！\n");
    printf("exit data :%d\n", *thread_retval);
*/
  
    //让主线程退出，不会影响其他正常运行的线程
    pthread_exit(NULL);

    return 0;   

}
