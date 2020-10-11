#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

int client_ID = 0;
int AutoMessage[10] = {0,0,0,0,0,0,1,2,3,4};

int main()
{
    //set socket object
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    char buf_answ_get = 0;
    char buf[20] = {0};
    //set network connection object
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1324);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //set 3s time out object
    struct timeval timeOut;
    timeOut.tv_sec = 3;
    timeOut.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut)) < 0)
    {
        printf("time out setting failed\n");
    }
    // mode for automatic and sequence_number for package validation
    int mode = 0; 
    int sequence_number = 1;
    int message_type = 0;

    printf("Input mode: ");
    scanf("%d", &mode);

    while(1)
    {
        // hand made
        if(mode == 0){
            printf("Input message：");
            scanf("%s", buf);
        }
        else if(mode == 1){
            if(message_type >= sizeof(AutoMessage) / sizeof(int)){
                break;
            }

            sprintf(buf, "Message %d", sequence_number);
            printf("Sending %s with type %d\n", buf, AutoMessage[message_type]);
        }
        
        // send message
        sendto(sockfd,&buf, sizeof(buf),0,(struct sockaddr*)&addr,sizeof(addr));

        socklen_t len=sizeof(addr);
        int receivePacketLen = recvfrom(sockfd,&buf_answ_get,sizeof(buf_answ_get),0,(struct sockaddr*)&addr,&len);
        //times out
        int time_out_counter = 0;
        if(receivePacketLen == -1 && errno == EAGAIN)      
        {
            printf("Server does not respond\n");
            continue;
        }

        if(66 == buf_answ_get)
        {
            printf("Message received\n");
        }
        else
        {
            printf("Message lost\n");
        }

        sequence_number += 1;
        message_type += 1;
    }
    close(sockfd);
}