#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "message.h"
#include "mydatabase.h"
#include<string.h>
#include<stdlib.h>

int main()
{
    // load data
    RegisterInfo database[1000];
    int datalen = load_data(database, 1000);
    printf("database length %d\n", datalen);
    //set socket object
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    int ret;
    char buf[DEFAULT_MSG_LEN] = {0};
    char sendback[DEFAULT_MSG_LEN] = {0};
    //应答字符设置为66
    char buf_answ=66;
    //set network connection object
    struct sockaddr_in cli;
    //create and deploy local network connection object
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //bind socket object and connection
    ret =bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(0 > ret)
    {
        printf("bind\n");
        return -1;
    }
    //set lenth
    socklen_t len=sizeof(cli);
    //wait for data
    //sequence number
    int pre_seq = 0;
    printf("waiting...\n");
    while(1)
    {
        //receive data
        recvfrom(sockfd, buf, DEFAULT_MSG_LEN, 0, (struct sockaddr*)&cli, &len);
        
        Message cur_pack = unpacking(buf);
        printf("recv query with phone number %u and technology %d\n", cur_pack.phone_number, 
                                    cur_pack.technology);
        printf("recv query with end_id %x and sequence number %d\n", cur_pack.end_id, 
                                    cur_pack.sequence_number);
        //send back ack
        // if(cur_pack.error_type == 0){
        //     if(cur_pack.sequence_number <= pre_seq){
        //         cur_pack.error_type = DUPLICATE_PACKET;
        //     }
        //     else if(cur_pack.sequence_number > pre_seq + 1){
        //         cur_pack.error_type = OUT_OF_SEQUENCE;
        //     }
        //     else if(cur_pack.sequence_number == pre_seq + 1){
        //         pre_seq += 1;
        //     }
        // }

        // Message resp = reply_data(cur_pack);
        // packing(resp, sendback);

        sendto(sockfd, &sendback, sizeof(sendback), 0, (struct sockaddr*)&cli,len);
    }
    //close socket
    close(sockfd);
}