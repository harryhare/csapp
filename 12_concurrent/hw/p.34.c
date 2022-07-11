/* 
 * vsum.c - A simple parallel sum program
 */
/* $begin vsum */
#include "csapp.h"
#include <math.h>
#include <assert.h>


//N*M
#define N 16
#define S 20
#define M 24

#define MAXTHREADS M

void *sum(void *vargp);

/* Global shared variables */
double vector1[N][S];          /* Vector */
double vector2[S][M];          /* Vector */
double result[N][M]; /* Partial sum computed by each thread */
const int nthreads = 4;
long nelems_per_thread = N / nthreads;  /* Region size for each thread */

int main(int argc, char **argv) {
    pthread_t tid[MAXTHREADS];
    int myid[MAXTHREADS];




    /* $begin psum */
    assert(N % nthreads == 0);
    nelems_per_thread = N / nthreads;

    /* Initialize vector */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < S; j++) {
            vector1[i][j] = i;
        }
    }
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < M; j++) {
            vector2[i][j] = i;
        }
    }

    /* Create threads and wait for them to finish */
    for (int i = 0; i < nthreads; i++) {
        myid[i] = i;
        Pthread_create(&tid[i], NULL, sum, &myid[i]);
    }
    for (int i = 0; i < nthreads; i++) {
        Pthread_join(tid[i], NULL);
    }


    /* Check final answer */
    printf("result:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%- 5.3lf\t", result[i][j]);
        }
        printf("\n");
    }

    exit(0);
}
/* $end psum */

/* Thread routine */
/* $begin psum-thread */
void *sum(void *vargp) {
    int myid = *((int *) vargp);             /* Extract the thread ID */
    long start_row = myid * nelems_per_thread;  /* Start element index */
    long end_row = start_row + nelems_per_thread;   /* End element index */
    for (int i = start_row; i < end_row; i++) {
        for(int j=0;j<M;j++){
            double sum=0;
            for(int k=0;k<S;k++){
                sum += vector1[i][k]*vector2[k][j];
            }
            result[i][j]=sum;
        }
    }
    return NULL;
}
/* $end psum-thread */

