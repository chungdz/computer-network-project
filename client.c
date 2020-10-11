#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    //创建socket对象
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	char buf_answ_get=0;
 	 char buf[1024]={0};
    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //set 3s time out object
    struct timeval timeOut;
    timeOut.tv_sec = 3;
    timeOut.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut)) < 0)
    {
        printf("time out setting failed\n");
    }

    while(1)
    {
        printf("Input message：");
      
        scanf("%s",buf);
        sendto(sockfd,&buf, sizeof(buf),0,(struct sockaddr*)&addr,sizeof(addr));

        socklen_t len=sizeof(addr);
        int receivePacketLen = recvfrom(sockfd,&buf_answ_get,sizeof(buf_answ_get),0,(struct sockaddr*)&addr,&len);
        //times out
        if (receivePacketLen == -1 && errno == EAGAIN)      
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
    }
    close(sockfd);
}