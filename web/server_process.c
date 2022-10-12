#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recycleChild(int arg){
    while(1){
        //pid = 1 回收所有的子进程
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1){
            //所有的子进程被回收完
            break;
        }else if(ret == 0){
            //还有子进程活着
            break;
        }else if (ret > 0){
            //被回收的子进程
            printf("子进程%d被回收了\n", ret);
        }
    }
}

int main(){
    //注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    sigaction(SIGCHLD, &act, NULL);

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

    //不断循环等待客户端连接
    while (1){
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);

        //记录了连接成功后客户端的地址信息IP port
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
        if (cfd == -1){
            if (errno == EINTR){
                continue;
            }
            perror("socket");
            exit(-1);
        }
        //每一个连接进来，创建一个子进程跟客户端通信
        pid_t pid = fork();
        if(pid == 0){
            //子进程
            //获取客户端的信息
            char cliIP[16];
            //获取IP并转换
            inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, cliIP, sizeof(cliIP));
            //获取端口号并转换
            unsigned short cliPort = ntohs(clientaddr.sin_port);
            printf("client IP is : %s, port is %d\n", cliIP, cliPort);

            //接收客户端发来的数据
            char recvBuf[1024] = {0};
            while (1){
                memset(recvBuf, 0, sizeof(recvBuf));
                int recvlen = read(cfd, &recvBuf, sizeof(recvBuf));
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
                int data = write(cfd, recvBuf, strlen(recvBuf) + 1);
                if (data < 0){
                    perror("write");
                    exit(-1);
                }
                printf("服务器端正常发送数据\n");

            }
        }

    }
    close(lfd);
    return 0;
}