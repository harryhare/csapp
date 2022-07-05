#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/sendfile.h>
// -m32 编译会报错, 需要安装依赖才能正常编译
// sudo apt-get install gcc-multilib

int main() {
    printf("test\n");
    char a[16];
    int fd = open("/tmp/tmp.txt", 0, 0);
//    read(fd,a,8);
//    write(1,a,8);
    sendfile(1, fd, 0, 8);
    return 0;
}