#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
    //创建socket对象
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    int ret;
    char buf[1024] ={0};
    //应答字符设置为66
    char buf_answ=66;
    //创建客户端网络通信对象
    struct sockaddr_in cli;
    //创建及设置本地网络通信对象
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //绑定socket对象与通信链接
    ret =bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(0>ret)
    {
        printf("bind\n");
        return -1;
    }
    //设置len
    socklen_t len=sizeof(cli);
    //循环等待数据
    while(1)
    {
        //接受数据
        recvfrom(sockfd,&buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
        //输出数据
        printf("recv string =%s\n",buf);
        //应答接受结果
        sendto(sockfd,&buf_answ,sizeof(buf_answ),0,(struct sockaddr*)&cli,len);
    }
    //关闭socket
    close(sockfd);
}