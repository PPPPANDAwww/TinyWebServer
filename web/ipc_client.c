#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    unlink("client.sock");

    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "client.sock");

    // 绑定本地套接字文件
    bind(cfd, (struct sockaddr *)&addr, sizeof(addr));

    // 连接服务器
    struct sockaddr_un seraddr;
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path, "server.sock");
    connect(cfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    // 通信
    int num = 0;
    while(1){
        // 发送数据
        char buf[128];
        sprintf(buf, "hello i am client %d\n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        printf("client send : %s\n", buf);

        //接收数据
        int recvlen = recv(cfd, buf, sizeof(buf),0);
        if(recvlen == -1){
            perror("recv");
            exit(0);
        }else if(recvlen == 0){
            printf("client closed...\n");
            break;
        }else if(recvlen > 0){
            printf("server say : %s\n", buf);
        }
        sleep(1);      

    }
    close(cfd);

    return 0;
    
}