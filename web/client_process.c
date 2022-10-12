#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    //1. 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        perror("socket");
        exit(-1);
    }
    printf("客户端创建套接字成功！\n");

    //2. 连接服务器端
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.107.128", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);

    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (ret == -1){
        perror("connect");
        exit(-1);
    }

    // 3. 通信
    char recvBuf[1024] = {0};
    int i = 0;

    while(1){
        memset(recvBuf, 0, sizeof(recvBuf));
        // sprintf函数调用的主要用途就是把一个字符串放在一个已知的字符数组里去。
        sprintf(recvBuf, "data : %d\n", i++);
        // 给服务端发送数据
        write(fd, recvBuf, strlen(recvBuf) + 1); //将字符结束符带上
        memset(recvBuf, 0, sizeof(recvBuf));

        // 读取
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if (len == -1){
            perror("read");
            exit(-1);
        }else if(len > 0){
            printf("recv server data : %s\n", recvBuf);
        }else if (len == 0){
            //表示客户单断开连接
            printf("server closed...");
            break;
        }
        
        //sleep(1);
    }

    //关闭文件描述符
    close(fd);

    return 0;

}