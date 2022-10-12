#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

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
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    //绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //监听
    listen(lfd, 8);

    //创建一个fd_set集合，存放需要监听的文件描述符
    fd_set rdset, tmp; //可以表示1024个,select的结果会对fd_set造成影响
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while (1){
        tmp = rdset;
        //调用select系统函数，让内核检测哪些文件描述符有数据
        ret  = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        }else if (ret == 0){
            continue;
        }else if (ret > 0){
            //说明检测到了有文件描述符对应的缓冲区的数据发生了改变
            if(FD_ISSET(lfd, &tmp)){
                //表示有新的客户端连接进来
                struct sockaddr_in cliaddr;
                int addrlen = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &addrlen);
                //将新的文件描述符
                FD_SET(cfd, &rdset);

                //更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            //遍历集合
            for (int i = lfd + 1; i <= maxfd; i++){
                if (FD_ISSET(i, &tmp)){
                    char buf[1024] = {0};
                    //从文件描述符i的文件读数据，放在缓冲区buf里
                    int len = read(i, buf, sizeof(buf));
                    if (len == -1){
                        perror("read");
                        exit(-1);
                    }else if (len == 0){
                        //对方断开连接
                        printf("client closed..\n");
                        FD_CLR(i, &rdset);
                        close(i);
                    }else if (len > 0){
                        printf("read data : %s", buf);

                        //从缓冲区向文件描述符i的文件写数据，回写数据
                        write(i, buf, strlen(buf) + 1);
                    }

                }

            }
        }


    }

    close(lfd);

    return 0;
}