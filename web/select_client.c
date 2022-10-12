#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    //创建socket
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in seraddr;
    inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr.s_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(9999);

    //连接服务器
    int ret = connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    if (ret == -1){
        perror("connect");
        exit(-1);
    }

    //通信
    int num  = 0;
    while(1){
        //发送数据
        char sendBuf[1024] = {0};
        sprintf(sendBuf, "send data %d\n", num++);
        write(fd, sendBuf, strlen(sendBuf) + 1);

        //接收
        int len = read(fd, sendBuf, sizeof(sendBuf));
        if (len == -1){
            perror("read");
            exit(-1);
        }else if(len > 0){
            printf("client receive buf = %s", sendBuf);
        }else{
            printf("服务器已断开连接...\n");
            break;
        }
        sleep(2);


    }
    close(fd);
    return 0;
}