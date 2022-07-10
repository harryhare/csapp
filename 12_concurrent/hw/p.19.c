/* 
 * Readers-writers solution with strong reader priority
*/
/* $begin rw2 */
#include "csapp.h"

/* Global variables */
int readcount;      /* Initially = 0 */
sem_t mutex, w, wg; /* All initially = 1 */

void reader(void) {
    while (1) {
        P(&mutex);
        readcount++;
        if (readcount == 1)
            P(&w);
        V(&mutex);

        /* Critical section: */
        /* Reading happens   */

        P(&mutex);
        readcount--;
        if (readcount == 0)
            V(&w);
        V(&mutex);
    }
}

void writer(void) {
    while (1) {
        P(&wg);
        P(&w);

        /* Critical section: */
        /* Writing happens   */

        V(&w);
        V(&wg);
    }
}
/* $end rw2 */
/*
 * 感觉这个大概有问题,就多了一层锁,好像也不解决问题啊...
 * 看了其他人答案,基本上是在 write的while循环里面价格 if 判断是不是已经有reader...
 */