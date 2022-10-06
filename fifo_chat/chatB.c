#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(){
    //1 判断有名管道文件是否存在，不存在则创建
    int ret = access("fifo1", F_OK);
    if(ret == -1){
        //文件不存在
        printf("管道1不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    //同上创建管道2
    ret = access("fifo2", F_OK);
    if(ret == -1){
        //文件不存在
        printf("管道2不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo2", 0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    //2 以只读的方式打开管道1
    int fdr = open("fifo1", O_RDONLY);
    if(fdr == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1成功，等待读取...\n");

    //3 以只写的方式打开管道2
    int fdw = open("fifo2", O_WRONLY);
    if(fdw == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2成功，等待写入...\n");

    char buf[128];

    // 4 循环的读写数据
    while (1){
        //读管道数据
        memset(buf, 0, 128);
        ret = read(fdr, buf, 128);
        if(ret <= 0){
            perror("read");
            break;
        }
        printf("buf: %s",buf);

        memset(buf, 0, 128);
        //获取键盘输入
        fgets(buf, 128, stdin);
        //写数据
        ret = write(fdw, buf, strlen(buf));
        if(ret == -1){
            perror("write");
            exit(0);
        }
        

        //6 关闭文件描述符
        close(fdw);
        close(fdr);
    }
    return 0;
}