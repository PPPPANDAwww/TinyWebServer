/*
int sem_init(sem_t *sem, int pshared, unsigned int value);
    -初始化信号量
    -参数：
        -sem：信号量变量的地址
        -pshared：0用在线程间，非0用在进程间
        -value：信号量中的值
int sem_wait(sem_t *sem);
    -对信号量的值-1，如果值为0，那么就阻塞
int sem_trywait(sem_t *sem);
    -尝试等待
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    -等待时间
int sem_post(sem_t *sem);
    -对信号量解锁，调用一次对信号量的值+1
int sem_getvalue(sem_t *sem);
    -获取值
int sem_destroy(sem_t *sem);
    -释放资源
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <semaphore.h>

//创建一个互斥量
pthread_mutex_t mutex;
//创建2个信号量
sem_t psem;
sem_t csem;

struct Node{
    int num;
    struct Node * next;
};
//头结点
struct Node *head = NULL;

void * producer(void * arg){
    //不断创建新的结点添加到容器中
    while(1){
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode ->next = head;
        head = newNode;
        newNode -> num = rand() % 1000;
        printf("add node, num :%d, tid: %ld\n",newNode->num, pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        sleep(2);
    }

    return NULL;
}

void * customer(void * arg){
    while(1){
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        //保存头节点指针
        struct Node * tmp = head;
        //不需要判断是否为空
        head = head->next;
        printf("delete node, num :%d, tid: %ld\n",tmp->num, pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(100);
    }

    return NULL;
}

int main(){
    //初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

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

    pthread_exit(NULL);

    return 0;
}