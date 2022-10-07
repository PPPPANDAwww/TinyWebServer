#include <stdio.h>
#include <string.h>

int main(){
    char * buf; //访问了非法内存
    strcpy(buf, "hello");

    return 0;

}