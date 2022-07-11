/* $begin tfgetsmain */
#include "csapp.h"

void* sleep5(void*){
    sleep(5);
}
void* mygets(void*){

}


char *tfgets(char *s, int size, FILE *stream){
    pthread_t tid;
    Pthread_create(&tid,NULL,sleep5,NULL);

}

int main()
{
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL)
        printf("BOOM!\n");
    else
        printf("%s", buf);

    exit(0);
}
/* $end tfgetsmain */
