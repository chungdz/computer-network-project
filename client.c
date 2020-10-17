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
#include "mydatabase.h"

int main()
{
    //load query
    Query q[1000];
    int qlen = load_query(q, 1000);
    printf("query length %d\n", qlen);
    //set socket object
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    unsigned int phone_number;
    unsigned int tech_tmp;
    unsigned char tech;
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
            printf("Input phone number and technology：");
            scanf("%u %d", &phone_number, &tech_tmp);
            tech = (unsigned char)tech_tmp;
        }
        else if(mode == 1){
            if(message_type >= qlen){
                break;
            }

            phone_number = q[message_type].phone_number;
            tech = q[message_type].tech;
            printf("Sending phone number %u with tech %d\n", phone_number, tech);
        }
        Message cur_msg = new_data(phone_number, tech, (char)sequence_number, ACC_PER);
        packing(cur_msg, send_buf);
        
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
            printf("Resending phone number %u with tech %d\n", phone_number, tech);
            sendto(sockfd, send_buf, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, sizeof(addr));
            socklen_t len=sizeof(addr);
            receivePacketLen = recvfrom(sockfd, &buf_answ_get, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&addr, &len);
        }

        Message server_pack = unpacking(buf_answ_get);
        if(server_pack.data_type == NOT_PAID){
            printf("Not paid received\n");
        }
        else if(server_pack.data_type == NOT_EXIST){
            printf("Not exist received\n");
        }
        else if(server_pack.data_type == ACCESS_OK){
            printf("Access ok received\n");
        }
        
        sequence_number += 1;
        message_type += 1;
        printf("\n");
    }
    close(sockfd);
}