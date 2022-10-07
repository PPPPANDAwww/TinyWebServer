#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

int main(){
    //1 打开一个文件
    int fd = open("test.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);

    //2 创建文件映射区
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //3  创建子进程
    pid_t pid = fork();
    if(pid > 0){
        //父进程
        wait(NULL);//回收子进程之后，再读取数据
        char buf[64];
        strcpy(buf, (char *)ptr);
        printf("read data : %s\n", buf);
    }else if (pid == 0){
        //子进程
        strcpy((char *)ptr, "nihao a, son!!!");
    }else{
        perror("fork");
        exit(0);
    }
    //关闭内存映射区
    munmap(ptr, size);

    return 0;
}