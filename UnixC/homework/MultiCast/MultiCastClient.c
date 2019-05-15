/** @file 
 *  @note 
 *  @brief UDP组播实现客户端探测服务器
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
#define MCAST_ADDR "224.0.0.66"  // 多播地址
#define MCAST_DATA "build"  // 多播内容

volatile bool timeout = false;

/* 计时线程函数 */
void *CountTime(void *arg)
{
    timeout = false;
    sleep(5);
    printf("time is out!\n");
    timeout = true;
}

int main(void)
{
    int sockfd;
    struct sockaddr_in stMcastAddr, stServerAddr;
    char szBuf[128];
    char szServerAddr[INET_ADDRSTRLEN];
    int iLenServerAddr = sizeof(stServerAddr);
    
    memset(szBuf, 0, sizeof(szBuf));
    memset(&stMcastAddr, 0, sizeof(stMcastAddr));
    memset(&stServerAddr, 0, iLenServerAddr);
    
    /* 设置socket为非阻塞 */
    sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (-1 == sockfd)
    { 
        perror("socket");
        return -1;
    }
    
    /* 填充多播地址 */
    stMcastAddr.sin_family = AF_INET;
    stMcastAddr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    stMcastAddr.sin_port = htons(MCAST_PORT);
    
    while (1)
    {   
        /* 向局域网内多播地址发送多播内容 */
        printf("开始发送多播数据，探测服务器...\n");
        int iRet = sendto(sockfd, MCAST_DATA, sizeof(MCAST_DATA), 0, (struct sockaddr*)&stMcastAddr, sizeof(stMcastAddr));
        if(0 > iRet)
        {
            perror("sendto");
            return -2;
        }
        else
        {
            /* 5s内未收到服务器端回复信息视为未找到服务器 */
            pthread_t tid;  ///计时线程id
            pthread_create(&tid, NULL, CountTime, NULL);
            pthread_detach(tid);///分离线程
            timeout = false;
            
            /* 计时5s内一直查收服务器发来的信息 */
            while (false == timeout)
            {
                iRet = recvfrom(sockfd, szBuf, sizeof(szBuf), 0, (struct sockaddr*)&stServerAddr, &iLenServerAddr);
                if (-1 != iRet)
                {
                    memset(szServerAddr, 0, sizeof(szServerAddr));                    
                    inet_ntop(AF_INET, &stServerAddr.sin_addr, szServerAddr, sizeof(szServerAddr));
                    printf("服务器IP: %s port: %d\n", szServerAddr, ntohs(stServerAddr.sin_port));
                }
            }
            printf("服务器查找完成\n");
        }
        printf("...\n\n\n");
    }
    
    return 0;
}
