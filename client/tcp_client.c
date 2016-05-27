#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char** argv){

    if(argc != 2){
        return 0;
    }
    struct hostent *host;
    printf("sizeof(struct hostent) = %d, sizeof(struct hostent*) = %d", sizeof(struct hostent), sizeof(struct hostent*));
    //memset(host, 0, sizeof(struct hostent*));
    host = gethostbyname(argv[1]);
    printf("h_name = %s\n", host->h_name);
    for(int i = 0; host->h_aliases[i] != NULL; i++)
        printf("h_aliases[%d] = %s\n", i, host->h_aliases[i]);
    printf("h_addrtype = %d\n", host->h_addrtype);
    printf("h_length = %d\n", host->h_length);
    struct in_addr addr;
    for(int i = 0; host->h_addr_list[i] != NULL; i++){
        //addr.s_addr = *(long *) host->h_addr_list[i];
        addr.s_addr =  inet_ntoa(host->h_addr_list[i]);
        printf("h_addr_list[%d] = %s\n", i, addr.s_addr;
    }
    struct in_addr __in;
    __in.s_addr = inet_addr(host->h_addr_list[0]);
    printf("inet_addr = %s\n", inet_ntoa(__in));
}

//adr_inet.sin_addr.s_addr =  inet_addr("127.0.0.95");
