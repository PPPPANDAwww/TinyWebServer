#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct sockInfo{
    int fd; //
    pthread_t tid;
    struct sockaddr_in addr;
};
struct sockInfo sockinfos[128];

void * working(void * arg){
    //子线程和客户端通信，cfd客户端的信息，线程号
    //获取客户端的信息
    struct sockInfo * pinfo = (struct sockInfo *)arg;
    char cliIP[16];
    //获取IP并转换
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIP, sizeof(cliIP));
    //获取端口号并转换
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("client IP is : %s, port is %d\n", cliIP, cliPort);

    //接收客户端发来的数据
    char recvBuf[1024] = {0};
    while (1){
        memset(recvBuf, 0, sizeof(recvBuf));
        int recvlen = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (recvlen == -1){
            perror("read");
             exit(-1);
        }else if(recvlen == 0){
            printf("client closed...\n");
            break;
        }
        //正常接收
        printf("recv client data : %s\n", recvBuf);
        memset(recvBuf,0, sizeof(recvBuf));
        sprintf(recvBuf, "成功收到客户端消息");
        //将收到的数据回射给客户端
        int data = write(pinfo->fd, recvBuf, strlen(recvBuf) + 1);
        if (data < 0){
            perror("write");
            exit(-1);
        }
    printf("服务器端正常发送数据\n");
    }
    close(pinfo->fd);
    return NULL;
}


int main(){
    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    //设置端口复用
    int val = 1;
    if(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1){
        perror("setsockopt");
        exit(-1);
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);

    //绑定
    if (bind(lfd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("bind");
        exit(-1);
    }

    //监听
    int ret = listen(lfd, 8);
    if (ret == -1){
        perror("bind");
        exit(-1);
    }
    //初始化数据
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < max; i++){
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    //循环等待客户端，每一个连接进来创建一个子进程跟客户端通信
    while(1){
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);

        //记录了连接成功后客户端的地址信息IP port
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
        if (cfd == -1){
            perror("socket");
            exit(-1);
        }
        struct sockInfo *pinfo;
        for (int i = 0; i < max; i++){
            //从数组中找到一个可用的sockInfo变量
            if (sockinfos[i].fd == -1){
                pinfo = &sockinfos[i];
                break;
            }
            if(i == max - 1){
                sleep(1);
                i--;
            }
        }

        pthread_t tid;
        pinfo->fd = cfd;
        memcpy(&pinfo->addr, &clientaddr, len);
        //创建子进程
        pthread_create(&pinfo->tid, NULL, working, NULL);

        //线程分离
        pthread_detach(pinfo->tid);

    }
    close(lfd);

    return 0;
}