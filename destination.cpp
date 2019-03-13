#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
int main(int argc, char**argv)
{
    int sockfd,sockfd2,n,n2,size;
    struct sockaddr_in servaddr,servaddr2,cliaddr,cliaddr2;
    socklen_t len,len2;
    char mesg[10000],mesg2[10000],result[10000],recvline[10000]; // result -> message from router1 + message from router2
    // socket config for router 1
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(33000);
    // assign address to socket (will be use for router 1)
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    // socket config for router 2 
    sockfd2 = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr2,sizeof(servaddr2));
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr2.sin_port = htons(41000);
    // assign address to socket (will be use for router 2)
    bind(sockfd2,(struct sockaddr *)&servaddr2,sizeof(servaddr2));

    for (;;) {
        len = sizeof(cliaddr);
        len2 = sizeof(cliaddr2);
        // recv message from router 1
        n = recvfrom(sockfd,mesg,10000,0,(struct sockaddr *)&cliaddr,&len);
        if(n>0){
        size = n;
        // router 1 message added to result message
        for(int i = 0;i<n;i++)result[i]=mesg[i];
        }
        // recv message from router 2
        n2 = recvfrom(sockfd2,mesg2,10000,0,(struct sockaddr *)&cliaddr2,&len2);
        if(n2>0){
            // router 2 message added to result message
            for(int i = 0;i<n2;i++)result[size+i]=mesg2[i];
            result[n2+size] = 0;
            printf("Result: %s \n",result);
            
            // send "Return message" to router2
            sendto(sockfd2,result,strlen(result),0,(struct sockaddr *)&cliaddr2,sizeof(cliaddr2));
        }

    }
}