#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "message.h"

int AutoMessage[10] = {0,0,0,0,0,0,1,2,3,4};

int main()
{
    //set socket object
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    char buf[1000] = {0};
    char send_buf[DEFAULT_MSG_LEN] = {0};
    char buf_answ_get[DEFAULT_MSG_LEN] = {0};
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

            sprintf(buf, "Message: %d", sequence_number);
            printf("Sending %s with type %d\n", buf, AutoMessage[message_type]);
        }
        Message cur_msg = new_data(buf, (char)sequence_number);
        packing(cur_msg, send_buf);
        break_message(send_buf, AutoMessage[message_type]);
        // send message
        sendto(sockfd, send_buf, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, sizeof(addr));
        // sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, sizeof(addr));

        socklen_t len=sizeof(addr);
        int receivePacketLen = recvfrom(sockfd, &buf_answ_get, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, &len);
        //times out
        int time_out_counter = 0;
        while(receivePacketLen == -1 && errno == EAGAIN)      
        {
            if(time_out_counter >= 3){
                printf("Server does not respond\n");
                exit(0);
            }
            time_out_counter += 1;
            printf("Resending %s with type %d\n", buf, AutoMessage[message_type]);
            sendto(sockfd, send_buf, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, sizeof(addr));
            socklen_t len=sizeof(addr);
            receivePacketLen = recvfrom(sockfd, &buf_answ_get, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, &len);
        }

        Message server_pack = unpacking(buf_answ_get);
        if(server_pack.data_type == MSG_ACK){
            printf("recv ACK with sequence number %d\n", server_pack.sequence_number);
            sequence_number += 1;
        }
        else if(server_pack.data_type == MSG_REJECT){
            char emsg[20];
            error_message(server_pack.reject_sub_node, emsg);
            printf("recv REJECT with sequence number %d and error type %s\n", server_pack.sequence_number, emsg);
        }
        message_type += 1;
        printf("\n");
    }
    close(sockfd);
}