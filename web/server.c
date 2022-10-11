//TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // 1. 创建socket（用于监听）
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1){
        perror("socket");
        exit(-1);
    }
    //使端口可以重复使用
    int iSockOptVal = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &iSockOptVal, sizeof(iSockOptVal)) == -1)
    {
        perror("setsockopt fail");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    //2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    //主机字节序转换为网络字节序-ip,协议，字段串，转换的结果
    inet_pton(AF_INET, "192.168.107.128", &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999); //主机字节序转换成网络字节序-端口
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1){
        perror("bind");
        exit(-1);
    }

    //3. 监听
    ret = listen(lfd, 8);
    if (ret == -1){
        perror("bind");
        exit(-1);
    }   

    //4. 接收客户端的连接
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    if (cfd == -1){
        perror("socket");
        exit(-1);
    }

    //输出客户端信息，网络字节序->主机字节序
    //xxx.xxx.xxx.xxx15个字符+结束符，16个字符
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    //5. 通信
    //获取客户端的数据
    char recvBuf[1024] = {0};
    while(1){
        int recvlen = read(cfd, recvBuf, sizeof(recvBuf));
        if (recvlen == -1){
            perror("read");
            exit(-1);
        }else if(recvlen > 0){
            printf("recv client data : %s\n", recvBuf);
        }else if (recvlen == 0){
            //表示客户单断开连接
            printf("client closed...");
            break;
        }
            //给客户端发送数据
        //char * data = "hello i am server\n";
        //write(cfd, data, strlen(data));

    }

    //关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}