
#include "csapp.h"

#define N 10

/* Global variables */
sem_t r;    /* Initially = 1 */
sem_t w;    /* Initially = 1 */

sem_t m_r_counter; /* Initially = 0 */
sem_t m_w_counter; /* Initially = 0 */

int write_count = 0;
int read_count = 0;

void reader(void) {
    while (1) {
        P(&r);
        P(&m_r_counter);//
        V(&r);  // 这句和上面那句颠倒好像也行
        read_count++;
        if (read_count == 1)
            P(&w);
        V(&m_r_counter);

        /* Critical section: */
        /* Reading happens   */

        P(&m_r_counter);
        read_count--;
        if (read_count == 0)
            V(&w);
        V(&m_r_counter);
    }
}

void writer(void) {
    int i;

    while (1) {
        P(&m_w_counter);
        write_count++;
        if (write_count == 1) {
            P(&r);
        }
        V(&m_w_counter);

        P(&w);

        /* Critical section: */
        /* Writing happens   */

        V(&w);

        P(&m_w_counter);
        if (write_count == 0) {
            V(&r);
        }
        V(&m_w_counter);
    }
}

/* https://blog.csdn.net/lixufeng1992/article/details/13615189
 */