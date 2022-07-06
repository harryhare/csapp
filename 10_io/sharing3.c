#include "csapp.h"

int main()
{
    int fd1, fd2;
    char c;

    fd1 = Open("foobar.txt", O_RDONLY, 0);
    fd2 = Open("foobar.txt", O_RDONLY, 0);
    Read(fd2, &c, 1); 
    Dup2(fd2, fd1);
    Read(fd1, &c, 1); 
    printf("c = %c\n", c);
    exit(0);
}

/* 文件内容:
 * foobar
 *
 * fd2 先读 `f`
 * 然后 fd1 定向到 fd2(fd2的内容给fd1)
 * 从 fd1 读,相当从 fd2 读
 * 读到第二个字符 `o`
 */