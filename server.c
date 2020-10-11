#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "message.h"
#include<string.h>

int main()
{
    //set socket object
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    int ret;
    char buf[DEFAULT_MSG_LEN] = {0};
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
    while(1)
    {
        //receive data
        recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&cli, &len);
        
        char *p = buf;

        Message cur_pack;
        cur_pack.error_type = 0;

        cur_pack.start_id  = *((short*) p);
        p += 2;
        cur_pack.client_id = *p;
        p += 1;
        cur_pack.data_type = *((short*) p);
        p += 2;
        cur_pack.sequence_number = *p;
        p += 1;
        cur_pack.length = *p;
        p += 1;

        cur_pack.message = p;
        int real_length = 0;
        char *tmp = p;
        while(tmp[real_length] != '\0'){
            real_length++;
        }
        real_length += 1;
        // printf("real length: %d\n", real_length);

        if(real_length != cur_pack.length){
            cur_pack.error_type = LENGTH_MISMATCH;
            cur_pack.length = real_length;
            printf("length error\n");
        }
        p += cur_pack.length;

        cur_pack.end_id = *((short*) p);
        if(cur_pack.end_id != END_PACKAGE){
            cur_pack.error_type = END_OF_PACKET_MISSING;
            printf("missing end\n");
        }
        printf("recv string: %s with sequence number %d and end id %x\n", cur_pack.message, 
                                    cur_pack.sequence_number, cur_pack.end_id);
        //send back ack
        char sendback[DEFAULT_MSG_LEN] = {0};
        sendto(sockfd, &buf_answ, sizeof(buf_answ), 0, (struct sockaddr*)&cli,len);
    }
    //close socket
    close(sockfd);
}