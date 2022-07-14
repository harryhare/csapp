/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
#include <assert.h>
#include "csapp.h"

enum Method {
    HEAD,
    GET,
    POST,
};

void doit(int fd);

void read_requesthdrs(rio_t *rp, char *headers, int *header_len);

void get_filetype(char *filename, char *filetype);

int parse_uri(char *uri, char *filename, char *cgiargs);

void serve_static(int fd, char *filename, int filesize, const char *echo_headers, enum Method method);

void serve_dynamic(int fd, char *filename, char *cgiargs, const char *echo_headers, enum Method method);

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

void sigchld_handler(int sig) //line:conc:echoserverp:handlerstart
{
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
} //line:conc:echoserverp:handlerend
void sigpipe_handler(int sig) //line:conc:echoserverp:handlerstart
{
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
} //line:conc:echoserverp:handlerend



int main(int argc, char **argv) {
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGPIPE, sigchld_handler);

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);                                             //line:netp:tiny:doit
        Close(connfd);                                            //line:netp:tiny:close
    }
}

/* $end tinymain */


/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    enum Method emethod;
    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("%s", buf);
    char headers[4096] = {0};
    int headers_len = strlen(buf)+5;
    strcpy(headers,"Echo-Request: ");
    strcpy(headers+14,buf);

    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
    if (strcasecmp(method, "GET") == 0) {
        emethod = GET;
    } else if (strcasecmp(method, "HEAD") == 0) {
        emethod = HEAD;
    } else if (strcasecmp(method, "POST") == 0) {
        emethod = POST;
    }
    if (emethod != GET && emethod != HEAD && emethod != POST) {                     //line:netp:doit:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                                                    //line:netp:doit:endrequesterr

    read_requesthdrs(&rio, headers, &headers_len);                              //line:netp:doit:readrequesthdrs
    assert(headers_len < 4096);

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);       //line:netp:doit:staticcheck
    if (stat(filename, &sbuf) < 0) {                     //line:netp:doit:beginnotfound
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    }                                                    //line:netp:doit:endnotfound

    if (is_static) { /* Serve static content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { //line:netp:doit:readable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size, headers, emethod);        //line:netp:doit:servestatic
    } else { /* Serve dynamic content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { //line:netp:doit:executable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        char *arg = cgiargs;
        if (emethod == POST) {
            char *p = strstr(headers, "Content-Length: ");
            int n = atoi(p + 16);
            if (n > 0) {
                arg = malloc(n);
                int r_n = Rio_readnb(&rio, arg, n);        //line:netp:readhdrs:checkterm
                assert(r_n == n);
                printf("%s\n", arg);
            }
        }
        serve_dynamic(fd, filename, arg, headers, emethod);            //line:netp:doit:servedynamic
        if (arg != cgiargs) {
            free(arg);
        }
    }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp, char *header, int *header_len) {
    char buf[MAXLINE];
    while (1) {          //line:netp:readhdrs:checkterm
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
        if (strcmp(buf, "\r\n") == 0) {
            break;
        }
        strcpy(header + *header_len, "Echo-");
        *header_len += 5;
        strcpy(header + *header_len, buf);
        *header_len += strlen(buf);
//        header[*header_len-1]=';';
//        header[*header_len]=0;
    }
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
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
/* $end parse_uri */


/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize, const char *echo_headers, enum Method method) {
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);       //line:netp:servestatic:getfiletype
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    strcat(buf, echo_headers);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    int r=rio_writen(fd, buf, strlen(buf));       //line:netp:servestatic:endserve
    printf("Response headers:\n");
    printf("%s", buf);
    if(r<=0){
        return;
    }

    if (method == HEAD) {
        return;
    }
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);    //line:netp:servestatic:open
//    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);//line:netp:servestatic:mmap
//    Close(srcfd);                           //line:netp:servestatic:close
//    Rio_writen(fd, srcp, filesize);         //line:netp:servestatic:write
//    Munmap(srcp, filesize);                 //line:netp:servestatic:munmap
    char *bb = malloc(filesize);
    int n = 0;
    while ((n = Rio_readn(srcfd, bb, filesize)) != 0) {
        int r=rio_writen(fd, bb, n);
        if(r<=0){
            return;
        }
    }
    free(bb);
    Close(srcfd);
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) {
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else if (strstr(filename, ".mpg"))
        strcpy(filetype, "video/mpg");
    else
        strcpy(filetype, "text/plain");
}
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs, const char *echo_headers, enum Method method) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    int r=rio_writen(fd, buf, strlen(buf));
    printf("%s", buf);
    if(r<=0){
        return;
    }
    sprintf(buf, "Server: Tiny Web Server\r\n");
    strcat(buf, echo_headers);
    r=rio_writen(fd, buf, strlen(buf));
    printf("%s", buf);
    if(r<=0){
        return;
    }
    if (method == HEAD) {
        return;
    }
    if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); //line:netp:servedynamic:setenv
        Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
        Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
}
/* $end serve_dynamic */

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
    int r=rio_writen(fd, buf, strlen(buf));
    if(r<=0){
        return;
    }
    sprintf(buf, "Content-type: text/html\r\n");
    r=rio_writen(fd, buf, strlen(buf));
    if(r<=0){
        return;
    }
    sprintf(buf, "Content-length: %d\r\n\r\n", (int) strlen(body));
    r=rio_writen(fd, buf, strlen(buf));
    if(r<=0){
        return;
    }
    r=rio_writen(fd, body, strlen(body));
    if(r<=0){
        return;
    }
}
/* $end clienterror */
/* test:
 * curl 'http://localhost:8080/cgi-bin/adder' -X POST --data-raw 'a=2&b=3' -v
 */