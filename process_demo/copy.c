//使用内存映射实现文件拷贝功能
/*
思路：
1、对原始的文件进行内存映射
2、创建一个新文件（拓展该文件）
3、把新文件的数据映射到内存中
4、通过内存拷贝将第一个文件的内存数据拷贝到新的文件内存中
5、释放资源
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

int main(){
    //1 对原始的文件进行内存映射
    int fd = open("youth.txt", O_RDWR);
    if (fd == -1){
        perror("open");
        exit(0);
    }
    //获取原始文件的大小
    int len =  lseek(fd, 0, SEEK_END);

    //2 创建一个新文件，拓展该文件
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1){
        perror("open");
        exit(0);
    }
    // 对新创建的文件进行拓展
    truncate("cpy.txt",len); //将文件拓展为指定长度
    write(fd1, " ", 1);

    //3 分别做内存映射
    void * ptr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    void * ptr1 = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);

    if (ptr == MAP_FAILED | ptr1 == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //内存拷贝
    memcpy(ptr1, ptr, len); //参数，要拷贝到的地址，要拷贝的地址，拷贝的长度

    //释放资源
    munmap(ptr1, len); //后打开的先释放
    munmap(ptr, len);

    return 0;
}