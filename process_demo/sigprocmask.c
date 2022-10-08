//编写一个程序，把所有的常规程序(1-31)的未决状态打印到屏幕
//设置某些信号是阻塞的，通过键盘产生这些信号

#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int main(){
    //设置2、3号信号阻塞
    sigset_t set;
    sigemptyset(&set); //清空

    //将2号和三号信号添加到信号集
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);
    int num = 0;

    while (1){
        num++;
        //获取当前的未决信号集的数据，设置一个接受的传出参数
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //遍历前32位
        for (int i = 1; i <= 31; i++){
            if (sigismember(&pendingset, i) == 1){
                printf("1");
            }else if(sigismember(&pendingset, i) == 0){
                printf("0");
            }else{
                perror("sigismember");
                exit(0);
            }
        }
        sleep(1);
        printf("\n");
        if (num == 10){
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }

    return 0;
}