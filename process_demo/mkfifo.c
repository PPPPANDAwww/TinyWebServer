/*
创建fifo文件
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
参数：
-pathname:管道名称的路径
-mode：文件的权限，和open的mode是一样的
返回值：成功返回0， 失败返回-1并设置错误号
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(){
    //1、判断文件是否存在
    int ret = access("fifo1", F_OK);
    
    if (ret == -1){
        printf("管道不存在，创建管道\n");

        //2、创建管道文件
        ret = mkfifo("test", 0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    return 0;
}