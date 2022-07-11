#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <malloc.h>

char *copy_charp(const char *p) {
    int str_len = strlen(p);
    char *r = malloc(sizeof(char) * (str_len + 1));
    memcpy(r, p, str_len);
    r[str_len] = 0;
    return r;
}

char **copy_charpp(const char **p) {
    char **r = NULL;
    int n = 0;
    for (n = 0; p[n] != NULL; n++);

    r = (char **) malloc((n + 1) * sizeof(char *));
    r[n] = NULL;
    for (int i = 0; i < n; i++) {
        r[i] = copy_charp(p[i]);
    }
    return r;
}

struct hostent *gethostbyname_ts(struct hostent *h, const char *name) {
    static pthread_mutex_t m;
    pthread_mutex_lock(&m);
    struct hostent *t = gethostbyname(name);
    memcpy(h, t, sizeof(struct hostent));
    h->h_name = copy_charp(t->h_name);
    h->h_aliases = copy_charpp(t->h_aliases);
    h->h_addr_list = copy_charpp(t->h_addr_list);
    pthread_mutex_unlock(&m);
    return h;
}

int main() {
    struct hostent t;
    struct hostent *h = gethostbyname_ts(&t, "www.baidu.com");
    printf("%s\n", h->h_name);
    printf("%d\n", h->h_length); // what length ?

    printf("alias:\n");
    for (int i = 0; h->h_aliases[i] != NULL; i++) {
        printf("%s\n", h->h_aliases[i]);
    }

    printf("type:\n");
    switch (h->h_addrtype) {
        case AF_INET:
            printf("AF_INET\n");
            break;
        case AF_INET6:
            printf("AF_INET6\n");
            break;
        default:
            printf(" %d\n", h->h_addrtype);
            break;
    }


    printf("address:\n");
    struct in_addr addr;
    for (int i = 0; h->h_addr_list[i] != NULL; i++) {
        if (h->h_addrtype == AF_INET) {
            addr.s_addr = *(u_long *) h->h_addr_list[i];
            printf("\tIPv4 Address #%d: %s\n", i, inet_ntoa(addr));
        } else if (h->h_addrtype == AF_INET6) {
            printf("\tRemotehost is an IPv6 address\n");
        }
    }

    return 0;
}