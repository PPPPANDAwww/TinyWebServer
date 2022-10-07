#include <unistd.h>
#include <stdio.h>

int main(){
    int seconds = alarm(5); //返回0
    printf("seconds = %d\n", seconds);

    sleep(2);
    seconds = alarm(10); //不阻塞
    printf("seconds = %d\n", seconds);

    int i = 0;

    while(1){
        //会运行10秒
        printf("i = %d\n",i++);
    }

    return 0;
}