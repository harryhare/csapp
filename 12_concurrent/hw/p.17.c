/* 
 * hellobug.c - "hello, world" program with a bug
 */
/* $begin hellobug */
/* WARNING: This code is buggy! */
#include "csapp.h"

void *thread(void *vargp);

int main() {
    pthread_t tid;

    Pthread_create(&tid, NULL, thread, NULL);
    //Pthread_join(tid,NULL);
    Pthread_exit(NULL);
    exit(0); //line:conc:hellobug:exit
}

/* Thread routine */
void *thread(void *vargp) {
    Sleep(1);
    printf("Hello, world!\n");
    return NULL;
}
/* $end hellobug */
