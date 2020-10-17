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
        Query rq;
        rq.phone_number = cur_pack.phone_number;
        rq.tech = cur_pack.technology;

        Message reply = cur_pack;
        reply.data_type = search_data(database, datalen, rq);
        packing(reply, sendback);

        sendto(sockfd, &sendback, sizeof(sendback), 0, (struct sockaddr*)&cli,len);
    }
    //close socket
    close(sockfd);
}