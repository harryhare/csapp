/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 */
#include <assert.h>
#include "csapp.h"

void doit(int fd);

void read_requesthdrs(rio_t *rp, char *headers, int *headers_len, char *host, int *content_length);

int parse_uri(char *uri, char *filename, char *cgiargs);

void serve_static(int fd, char *filename, int filesize);

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

void *thread(void *vargp);


/* test
 * curl -x http://local:8080  ifconfig.me
 */

int main(int argc, char **argv) {
    int listenfd, *connfdp;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfdp = Malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}
/* $end tinymain */

/* Thread routine */
void *thread(void *vargp) {
    int connfd = *((int *) vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    doit(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE];
    rio_t rio_client;
    rio_t rio_server;
    char request[MAXLINE];
    char response[MAXLINE];


    Rio_readinitb(&rio_client, fd);
    if (!Rio_readlineb(&rio_client, request, MAXLINE))
        return;
    printf("%s", request);
    sscanf(request, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }
    char headers[4096]; //end with \r\n\r\n
    char host[256];
    int headers_len = 0;
    int content_length = 0;
    read_requesthdrs(&rio_client, headers, &headers_len, host, &content_length);


    char *port = strchr(host, ':');
    if (port == NULL) {
        port = "80";
    } else {
        *port = 0;
        port++;
    }
    printf("uri: %s\n", uri);
    printf("host: %s\n", host);
    printf("port: %s\n", port);
    int clientfd = Open_clientfd(host, port);
    if (clientfd > 0) {
        printf("open server fd succ\n");
    } else {
        printf("open server fd err\n");
    }
    Rio_readinitb(&rio_server, clientfd);
    rio_writen(clientfd, request, strlen(request));
    rio_writen(clientfd, headers, headers_len);
    long n = 0;
    if (content_length > 0) {
        puts("send:");
        n = rio_readnb(&rio_client, buf, content_length);
        puts(buf);
        Rio_writen(clientfd, buf, n);
    }
    puts("recv:");

    puts("reponse line:");
    n = Rio_readlineb(&rio_server, response, MAXLINE);
    puts(response);
    Rio_writen(fd, response, n);

    puts("reponse header:");
    headers_len = 0;
    read_requesthdrs(&rio_server, headers, &headers_len, host, &content_length);
    Rio_writen(fd, headers, headers_len);

    if (content_length > 0) {
        printf("response body: ...%d\n", content_length);
        char *body = malloc(content_length);
        n = rio_readnb(&rio_server, body, content_length);
        assert(n == content_length);
        //printf("%s\n", buf);
        Rio_writen(fd, body, n);
        free(body);
    }
    Close(clientfd);
    //serve_static(fd, filename, sbuf.st_size);
}

/* $end doit */


void read_requesthdrs(rio_t *rp, char *headers, int *headers_len, char *host, int *content_length) {
    char buf[MAXLINE];
    while (1) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
        strcpy(headers + *headers_len, buf);
        *headers_len += strlen(buf);

        if (strncasecmp("host: ", buf, 6) == 0) {
            int host_len = strlen(buf + 6);
            strcpy(host, buf + 6);
            host[host_len - 2] = 0;//\r\n => \0\n
        }
        if (strncasecmp("content-length: ", buf, 16) == 0) {
            *content_length = atoi(buf + 16);
        }
        if (strcmp(buf, "\r\n") == 0) {
            break;
        }
    }
    return;
}


int parse_uri(char *uri, char *filename, char *cgiargs) {
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
        strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
        strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
        strcat(filename, uri);                           //line:netp:parseuri:endconvert1
        if (uri[strlen(uri) - 1] == '/')                   //line:netp:parseuri:slashcheck
            strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
        return 1;
    } else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
        ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        } else {
            strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
        }
        strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
        strcat(filename, uri);                           //line:netp:parseuri:endconvert2
        return 0;
    }
}


void serve_static(int fd, char *filename, int filesize) {
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, 2);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, "text");
    Rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
    printf("Response headers:\n");
    printf("%s\n", buf);


    Rio_writen(fd, "ok", 2);         //line:netp:servestatic:write
}


/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int) strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */

/* test
 * curl -x http://localhost:8080  ifconfig.me -v
 * curl --proxy http://localhost:8080  ifconfig.me -v
 */