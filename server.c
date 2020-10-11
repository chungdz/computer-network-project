#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    //set socket object
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    int ret;
    char buf[1024] ={0};
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
        recvfrom(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
        //print reveived data
        printf("recv string: %s\n",buf);
        //send back ack
        sendto(sockfd,&buf_answ,sizeof(buf_answ),0,(struct sockaddr*)&cli,len);
    }
    //close socket
    close(sockfd);
}