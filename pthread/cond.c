/*
条件变量类型为 pthread_cond_t。
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t * attr);  //初始化一个条件变量
int pthread_cond_destroy(pthread_cond_t *cond);// 销毁一个条件变量   
int pthread_cond_signal(pthread_cond_t *cond); 
    -唤醒至少一个阻塞在条件变量上的线程
int pthread_cond_broadcast(pthread_cond_t *cond); 
    -唤醒全部阻塞在条件变量上的线程  
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex); 
    -等待，阻塞函数，调用了该函数线程会阻塞
int pthread_cond_timewait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);  
    -等待多长时间，调用了这个函数，线程会阻塞，直到指定的时间结束

*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>

//创建一个互斥量
pthread_mutex_t mutex;
//创建条件变量
pthread_cond_t cond;

struct Node{
    int num;
    struct Node * next;
};
//头结点
struct Node *head = NULL;

void * producer(void * arg){
    //不断创建新的结点添加到容器中
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode ->next = head;
        head = newNode;
        newNode -> num = rand() % 1000;
        printf("add node, num :%d, tid: %ld\n",newNode->num, pthread_self());

        //只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        sleep(3);
    }

    return NULL;
}

void * customer(void * arg){
    while(1){
        pthread_mutex_lock(&mutex);
        //保存头节点指针
        struct Node * tmp = head;
        //判断是否有数据
        if(head != NULL){
            //有数据
            head = head->next;
            printf("delete node, num :%d, tid: %ld\n",tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }else{
            //没有数据，需要等待
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);

        }
    }

    return NULL;
}

int main(){
    //初始化
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //创建5个消费者线程，和5个生产者线程
    pthread_t ptids[5], ctids[5];
    for(int i = 0; i< 5; i++){
        pthread_create(&ptids[i], NULL, producer, NULL);
    }
    for(int i = 0; i< 5; i++){
        pthread_create(&ctids[i], NULL, customer, NULL);
    }
    //线程分离
    for(int i = 0; i< 5; i++){
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }    
    while(1){
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);


    return 0;
}