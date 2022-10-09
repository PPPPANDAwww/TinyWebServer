/*
生产者消费者模型（粗略的版本）
更新：没有数据，阻塞，等待信号通知数据到达
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>

pthread_mutex_t mutex;

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
            usleep(100);
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(){
    pthread_mutex_init(&mutex, NULL);


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