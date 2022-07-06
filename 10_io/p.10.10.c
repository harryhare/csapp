/*
 *  use dup to redirect
 */

#include "csapp.h"

int main(int argc, char **argv) 
{
    int n;
    rio_t rio;
    char buf[MAXLINE];
    // insert begein
    if(argc>1){
        char * filename=argv[1];
        int fd=open(filename,O_RDONLY);
        dup2(fd,STDIN_FILENO);
    }
    //insert end

    Rio_readinitb(&rio, STDIN_FILENO);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) 
	Rio_writen(STDOUT_FILENO, buf, n);
    /* $end cpfile */
    exit(0);
    /* $begin cpfile */
}




