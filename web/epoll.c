#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

int main(){
    // 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1){
        perror("socket");
        exit(-1);
    }
    printf("套接字创建成功\n");

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 监听
    listen(lfd, 8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);

    // 将监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events =EPOLLIN | EPOLLOUT;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    // 接收检测后的数组
    struct epoll_event epevs[1024];

    while(1){

        // 检测哪些有数据
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1){
            perror("epoll_wait");
            exit(-1);
        }
        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++){
            int curfd = epevs[i].data.fd;
            if(curfd == lfd){
                // 监听的文件描述符有数据到达，有客户端连接
                struct sockaddr_in cliaddr;
                int addrlen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &addrlen);

                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else{
                if(epevs[i].events & EPOLLOUT){
                    continue;
                }else{
                    // 有数据到达，需要通信
                    char buf[1024] = {0};
                    int len = read(curfd, buf, sizeof(buf));
                    if (len == -1){
                        perror("read");
                        exit(-1);
                    }else if (len == 0){
                        // 对方断开连接
                        printf("client closed..\n");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, &epev);
                        close(curfd);
                    }else if (len > 0){
                        printf("read data : %s", buf);
                        // 从缓冲区向文件描述符i的文件写数据，回写数据
                        write(curfd, buf, strlen(buf) + 1);
                    }
                }                
            }
        }

    }

    close(lfd);
    close(epfd);

    return 0;
}