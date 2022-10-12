/*
#include <poll.h>
struct pollfd{
    int fd; //委托内核检测的文件描述符
    short events; //委托内核检测文件描述符的什么事件
    short revents; //文件描述符实际发生的事件
}；
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
-参数：
    -fds：是一个struct pollfd结构体数组，这是一个需要检测的文件描述符的集合
    -nfds:这个是第一个参数数字中最后一个有效元素的下标
    -timeout：阻塞时长
        0：不阻塞
        -1：阻塞，当检测到需要检测的文件描述符有变化，解除阻塞
        >0：阻塞的时长


*/
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

int main(){
    //  创建socket
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

    //绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //监听
    listen(lfd, 8);

    //初始化检测的文件描述符数组
    struct pollfd fds[1024];
    for(int i = 0; i < 1024; i++){
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;

    int nfds = 0;

    while (1){

        //调用poll系统函数，让内核检测哪些文件描述符有数据,-1表示阻塞
        int ret  = poll(fds, nfds + 1, -1);
        if(ret == -1){
            perror("poll");
            exit(-1);
        }else if (ret == 0){
            continue;
        }else if (ret > 0){
            //说明检测到了有文件描述符对应的缓冲区的数据发生了改变
            if(fds[0].revents & POLLIN){
                //表示有新的客户端连接进来
                struct sockaddr_in cliaddr;
                int addrlen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &addrlen);

                //将新的文件描述符
                for(int i = 1; i < 1024; i++){
                    if(fds[i].fd == -1){
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                //更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd;
            }
            //遍历集合
            for (int i = 1; i <= nfds; i++){
                if (fds[i].revents & POLLIN){
                    // 说明这个文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if (len == -1){
                        perror("read");
                        exit(-1);
                    }else if (len == 0){
                        //对方断开连接
                        printf("client closed..\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }else if (len > 0){
                        printf("read data : %s", buf);
                        //从缓冲区向文件描述符i的文件写数据，回写数据
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }

                }

            }
        }


    }

    close(lfd);

    return 0;
}