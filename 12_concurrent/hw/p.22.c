/* $begin select */
#include "csapp.h"

int echo(int connfd);

void command(void);

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);  //line:conc:select:openlistenfd

    FD_ZERO(&read_set);              /* Clear read set */ //line:conc:select:clearreadset
    FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ //line:conc:select:addstdin
    FD_SET(listenfd, &read_set);     /* Add listenfd to read set */ //line:conc:select:addlistenfd

    int max_fd = listenfd;
    while (1) {
        ready_set = read_set;
        Select(max_fd + 1, &ready_set, NULL, NULL, NULL); //line:conc:select:select
        if (FD_ISSET(STDIN_FILENO, &ready_set)) //line:conc:select:stdinready
            command(); /* Read command line from stdin */
        if (FD_ISSET(listenfd, &ready_set)) { //line:conc:select:listenfdready
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
            FD_SET(connfd, &read_set);
            if (connfd > max_fd) {
                max_fd = connfd;
            }
//            echo(connfd); /* Echo client input until EOF */
//            Close(connfd);
        }
        for (int fd = listenfd + 1; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &ready_set)) {
                int ret = echo(fd);
                if (ret <= 0) {
                    FD_CLR(fd, &read_set);// max_fd 没法回收
                    Close(fd);
                }
            }
        }

    }
}

int echo(int connfd) {
    int n = 0;
    char buf[MAXLINE];
    long ret = 0;
    while ((ret = read(connfd, buf + n, 1)) == 1) {
        n++;
        if (buf[n-1] == '\n') {
            write(connfd, buf, n);
            return 1;
        }
    }
    return (int) (ret);
}

void command(void) {
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
        exit(0); /* EOF */
    printf("%s", buf); /* Process the input command */
}
/* $end select */


