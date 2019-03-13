#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <unistd.h>

int main(int argc, char**argv)
{
    double cpu_time_used;
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[10000];
    char recvline[10000];
    if (argc != 2)
    {
        printf("usage: source <Message>\n");
        exit(1);
    }
    // Socket config to send message to broker
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("10.10.1.2");
    servaddr.sin_port=htons(32000);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    // create message
    strcpy(sendline,argv[1]);
    // start the calculating time
    auto start = std::chrono::steady_clock::now();
    sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
    /* Return Message */
    n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
    recvline[n]=0;
    // end the calculating time when message come back from destination through whole path.
    auto end = std::chrono::steady_clock::now();
    cpu_time_used = std::chrono::duration_cast<std::chrono::milliseconds>(end -start).count();
    printf("End-to-end Delay: %f \n",cpu_time_used/2);
}