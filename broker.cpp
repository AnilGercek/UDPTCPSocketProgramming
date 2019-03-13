#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <chrono>
int main(int argc, char**argv)
{
    int listenfd,connfd,n,n2;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen;
    pid_t childpid;
    char mesg[10000],recvline[10000];
    // Socket config to get message from source
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(32000);
    // assign address to socket 
    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(listenfd,1024);
    while(true)
    {
    clilen=sizeof(cliaddr);
    connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
        if ((childpid = fork()) == 0)
        {
            close (listenfd);
            while(true)
            {
                // wait for message from source
                n = recvfrom(connfd,mesg,10000,0,(struct sockaddr *)&cliaddr,&clilen);
                if(n!=0)
                {
                    printf("-----------\n");
                    mesg[n] = 0;
                    printf("Received the following:\n");
                    printf("%s \n",mesg);
                    printf("-----------\n");
                    int meslen = n/2;
                    char message1[meslen+1];
                    char message2[n-meslen+1];
                    // Divide message to 2 part 
                    for(int i = 0;i<n;i++)
                    {
                        if(i<meslen)
                            message1[i] = mesg[i];
                        else
                            message2[i-meslen] = mesg[i];
                    }
                    message1[meslen] = 0;
                    message2[n-meslen] =0;
                    int sockfd2,sockfd3;
                    struct sockaddr_in servaddr2,servaddr3;
                    // Socket config to send message to router 1
                    sockfd2=socket(AF_INET,SOCK_DGRAM,0);
                    if(sockfd2==-1){printf("socket error: %s \n",strerror(errno));}
                    bzero(&servaddr2,sizeof(servaddr2));
                    servaddr2.sin_family = AF_INET;
                    servaddr2.sin_addr.s_addr=inet_addr("10.10.2.2");
                    servaddr2.sin_port=htons(41000);
                    connect(sockfd2, (struct sockaddr *)&servaddr2, sizeof(servaddr2));
                    // send the second part to router2 
                    int e = sendto(sockfd2,(char*)message1,strlen(message1),0,(struct sockaddr *)&servaddr2,sizeof(servaddr2));
                    if(e==-1)printf("sendto error: %s \n",strerror(errno));


                    // Socket config to send message to router 2
                    sockfd3=socket(AF_INET,SOCK_DGRAM,0);
                    bzero(&servaddr3,sizeof(servaddr3));
                    servaddr3.sin_family = AF_INET;
                    servaddr3.sin_addr.s_addr=inet_addr("10.10.4.2");
                    servaddr3.sin_port=htons(42000);
                    auto start = std::chrono::steady_clock::now();
                    // send the second part to router2 
                    sendto(sockfd3,message2,strlen(message2),0,(struct sockaddr *)&servaddr3,sizeof(servaddr3));
                    // Return Message
                    n2=recvfrom(sockfd3,recvline,10000,0,NULL,NULL);
                    auto end = std::chrono::steady_clock::now();
                    double cpu_time_used = std::chrono::duration_cast<std::chrono::milliseconds>(end -start).count();
                    printf("b-to-d Delay: %f \n",cpu_time_used/2);
                    recvline[n2]=0;
                    sendto(connfd,recvline,n2,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
                    close(sockfd2);
                    close(sockfd3);
                }
            }
        }
    }
    close(connfd);
}