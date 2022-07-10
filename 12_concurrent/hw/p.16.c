/* 
 * hello.c - Pthreads "hello, world" program 
 */
/* $begin hello */
#include "csapp.h"

void *thread(void *vargp);                    //line:conc:hello:prototype

int main(int argc, char **argv)                                    //line:conc:hello:main
{
    if (argc <= 1) {
        exit(-1);
    }
    int n = atoi(argv[1]);
    pthread_t *tids = malloc(n * sizeof(pthread_t));
    for (int i = 0; i < n; i++) {
        Pthread_create(&tids[i], NULL, thread, NULL); //line:conc:hello:create
    }
    for (int i = 0; i < n; i++) {
        Pthread_join(tids[i], NULL);                  //line:conc:hello:join
    }
    exit(0);                                  //line:conc:hello:exit
}

void *thread(void *vargp) /* thread routine */  //line:conc:hello:beginthread
{
    printf("Hello, world!\n");
    return NULL;                               //line:conc:hello:return
}                                              //line:conc:hello:endthread
/* $end hello */
