#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    unlink("server.sock");
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");

    bind(lfd, (struct sockaddr *)&addr, sizeof(addr));

    listen(lfd, 100);

    //等待连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, (socklen_t *)&len);
    printf("client socket filename : %s\n", cliaddr.sun_path);

    //通信
    while(1){
        char buf[128];
        int recvlen = recv(cfd, buf, sizeof(buf),0);
        if(recvlen == -1){
            perror("recv");
            exit(0);
        }else if(recvlen == 0){
            printf("client closed...\n");
            break;
        }else if(recvlen > 0){
            printf("client say : %s\n", buf);
            send(cfd, buf, len, 0);

        }

    }
    close(cfd);
    close(lfd);
    return 0;
    
}