/** @file 
 *  @note 
 *  @brief UDP组播服务器端实现
 *
 *  @author     
 *  @date       
 *
 *  @note 
 *
 *  @warning 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MCAST_PORT 10365
#define MCAST_ADDR "224.0.0.66" /*一个局部连接多播地址，路由器不进行转发*/
#define BUFF_SIZE 256   //接收缓冲区大小

int main()
{
    struct sockaddr_in stClientAddr;
    struct sockaddr_in stServerAddr;
    int err = -1;
    char szClientAddr[INET_ADDRSTRLEN];
    socklen_t iLenClientAddr = sizeof(stClientAddr);;
    char buff[BUFF_SIZE];
    int iRet = 0;
    
    memset(&stServerAddr, 0, sizeof(stServerAddr));
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd)
    {
        perror("socket");
        return -1;
    }

    /*初始化地址*/
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stServerAddr.sin_port = htons(MCAST_PORT);

    /*绑定socket*/
    err = bind(sockfd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr));
    if (0 > err)
    {
        perror("bind");
        return -2;
    }

    /*设置回环许可*/
    int iLoop = 1;
    err = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &iLoop, sizeof(iLoop));
    if (err<0)
    {
        perror("setsockopt");
        return -3;
    }

    struct ip_mreq stMreq;/*加入多播组*/
    stMreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);//多播地址
    stMreq.imr_interface.s_addr = htonl(INADDR_ANY); //网络接口为默认

    /*将本机加入多播组*/
    err = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq, sizeof(stMreq));
    if (0 > err)
    {
        perror("setsockopt");
        return -4;
    }
    
    /*循环接收多播组的消息*/
    while (1)
    {
        memset(&stClientAddr, 0, iLenClientAddr);
        memset(buff, 0, BUFF_SIZE);

        iRet = recvfrom(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
        if(-1 == iRet)
        {
            perror("recvfrom");
            return -5;
        }
        
        /*打印服务器收到的多播信息*/
        printf("buffer: %s\n",buff);
        if(0 == strncmp(buff, "build", 5))
        {
            memset(szClientAddr, 0, sizeof(szClientAddr)); 
            inet_ntop(AF_INET, &stClientAddr.sin_addr, szClientAddr, sizeof(szClientAddr));
            printf("client IP: %s Port: %d\n", szClientAddr, ntohs(stClientAddr.sin_port));

            sendto(sockfd, "ok", 2,  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
        }
    }

    /*退出多播组*/
    err = setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreq, sizeof(stMreq));
    close(sockfd);

    return 0;
}
