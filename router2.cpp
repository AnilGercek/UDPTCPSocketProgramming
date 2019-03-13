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
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    char mesg[10000];
    // socket config for broker node (coming message)
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(42000);
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    
    int sockfd2,n2;
    struct sockaddr_in servaddr2,cliaddr2;
    char recvline[10000];
    // socket config for destination node (Sending message)
    sockfd2=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr2,sizeof(servaddr));
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_addr.s_addr=inet_addr("10.10.3.2");
    servaddr2.sin_port=htons(41000);
    for (;;)
    {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd,mesg,10000,0,(struct sockaddr *)&cliaddr,&len);
        if(n>0)
        {
        // print the router 2 part of the message
        printf("---------------\n");
        mesg[n] = 0;
        printf("Received the following:\n");
        printf("%s \n",mesg);
        printf("---------------\n");
        auto start = std::chrono::steady_clock::now();
        // send the part of the message to destination
        sendto(sockfd2,mesg,strlen(mesg),0,(struct sockaddr *)&servaddr2,sizeof(servaddr2));
        // Wait for return message
        n=recvfrom(sockfd2,recvline,10000,0,NULL,NULL);
        auto end = std::chrono::steady_clock::now();
        double cpu_time_used = std::chrono::duration_cast<std::chrono::milliseconds>(end -start).count();
        printf("r-to-d Delay: %f \n",cpu_time_used/2);
        recvline[n]=0;
        // send return message to broker
        sendto(sockfd,recvline,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        }
    }
}