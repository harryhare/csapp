#include <stdio.h>
#include <arpa/inet.h>

char * get_ip_string(uint32_t a){
    static char buf[20];
    struct in_addr addr;
    addr.s_addr= htonl(a);
    inet_ntop(AF_INET,&addr,buf,20);
    return buf;
}
uint32_t get_ip_addr(const char * buf){
    struct in_addr addr;
    inet_pton(AF_INET,buf,&addr);
    //inet_aton(buf,&addr);
    return ntohl(addr.s_addr);
}

int main(){
    printf("%s\n", get_ip_string(0x0));
    printf("%s\n", get_ip_string(0xFFFFFF00));
    printf("%s\n", get_ip_string(0x0A010140));

    printf("0x%08x\n", get_ip_addr("107.212.122.205"));
    printf("0x%08x\n", get_ip_addr("64.12.149.13"));
    printf("0x%08x\n", get_ip_addr("107.212.96.29"));
    return 0;
}