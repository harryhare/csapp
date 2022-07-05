//
// Created by mayue on 5/17/22.
//

#include "csapp.h"
int main(){
    int fd1,fd2;
    //printf("%d,%d,%d",stdin,stdout,stderr);
    fd1=Open("foo.txt",O_RDONLY,0);
    Close(fd1);
    fd2=Open("baz.txt",O_RDONLY,0);
    printf("%d,%d\n",fd1,fd2);
    exit(0);
}