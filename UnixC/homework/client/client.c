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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "common.h"
#include "tcpcli.h"
#include "udpcli.h"

#define MCAST_PORT 10365
#define MCAST_ADDR "224.0.0.66"  // 多播地址
#define MCAST_DATA "build"  // 多播内容

volatile bool timeout = false;

/* 计时线程函数 */
void *CountTime(void *arg)
{
    timeout = false;
    sleep(1);
    printf("Out of time!\n");
    timeout = true;
}

uint16_t ServerSearch(stServerNode *pHead)
{
    int sockfd;
    struct sockaddr_in stMcastAddr, stServerAddr;
    char szBuf[128];
    char szServerAddr[INET_ADDRSTRLEN];
    int iLenServerAddr = sizeof(stServerAddr);
    uint16_t usiServerPort = 0;
    
    memset(szBuf, 0, sizeof(szBuf));
    memset(&stMcastAddr, 0, sizeof(stMcastAddr));
    memset(&stServerAddr, 0, iLenServerAddr);
    
    /* 设置socket为非阻塞 */
    sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (-1 == sockfd)
    { 
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }
    
    /* 填充多播地址 */
    stMcastAddr.sin_family = AF_INET;
    stMcastAddr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    stMcastAddr.sin_port = htons(MCAST_PORT);
    
    //while (1)
    {   
        /* 向局域网内多播地址发送多播内容 */
        printf("开始发送多播数据，探测服务器...\n");
        int iRet = sendto(sockfd, MCAST_DATA, sizeof(MCAST_DATA), 0, (struct sockaddr*)&stMcastAddr, sizeof(stMcastAddr));
        if(0 > iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return;
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
                    usiServerPort = ntohs(stServerAddr.sin_port);
                    printf("服务器IP: %s port: %hu\n", szServerAddr, usiServerPort);

                    if (false == IsExist(pHead, szServerAddr))///检查链表中IP是否存在
                    {
                        AddNode(pHead, szServerAddr, usiServerPort);
                    }
                }
            }
        }
    }

    return CountNodes(pHead);
}

int main(int argc, char* argv[])
{
    char cTemp = '\0';
    char cBuf;              //用于清空输入尾部的换行符
    uint16_t usiNum = 0;    //选择服务编号变量
    uint16_t usiNodes = 0;  //存储在线服务器总数
    stServerNode *pstServer = NULL; //用于存储待连接的服务器信息

    stServerNode *pHead = (stServerNode*)malloc(sizeof(stServerNode));
    if (NULL == pHead)
    {
        printf("内存申请失败\n");
        return 1;
    }

    pHead->pstNext = NULL;

    while (0 == (usiNodes = ServerSearch(pHead)))
    {
        printf("未找到服务器，是否继续搜索？是(Y)/任意键结束搜索并退出程序:");
        scanf("%c", &cTemp);
        while (((cBuf = getchar()) != EOF) && (cBuf != '\n'));//使用getchar()获取输入缓冲区字符，直到获取的c是'\n'或文件结尾符EOF为止

        if ('Y' == cTemp)
        {
            cTemp = '\0';
            continue;
        }
        else
        {
            printf("未找到服务器，用户退出！\n");
            return 0;
        }
    }

    printf("**********找到的服务器列表************\n");
    PrintNode(pHead);
    printf("*************服务器列表***************\n");
    
    printf("选择需要连接服务器序号:\n");
    scanf("%hu", &usiNum);
    while (usiNum <= 0 || usiNum > usiNodes)
    {
        printf("没有您要连接的服务器，请重新输入:\n");
        scanf("%hu", &usiNum);
        while (((cBuf = getchar()) != EOF) && (cBuf != '\n'));
    }

    pstServer = FindNode(pHead, usiNum);
    printf("准备连接服务器[%s:%hu]\n", pstServer->pszIP, pstServer->usiPort);

    /* 全局变量内存申请 */
    //文件信息结构
    g_pstComTransInfo = (COM_TRANS_INFO_S*)malloc(sizeof(COM_TRANS_INFO_S));
    if (NULL == g_pstComTransInfo)
    {
        printf("内存申请失败\n");
        return 1;
    }

    //发送或接收缓存
    g_pszTransBuf = (char*)malloc(BUFFER_SIZE);
    if(NULL == g_pszTransBuf)
    {
        printf("内存申请失败！\n");
        return;
    }

    /* 全局变量内存初始化 */
    memset((void*)g_pstComTransInfo, 0, sizeof(COM_TRANS_INFO_S));
    memset(g_pszTransBuf, 0, BUFFER_SIZE);

    ProtocolMenu();
    scanf("%hu", &usiNum);
	
    if (1 == usiNum)
    {
        UDPService(pstServer);
    }
    else
    {
        TCPService(pstServer);
    }

    free(g_pstComTransInfo);
    g_pstComTransInfo = NULL;
    free(g_pszTransBuf);
    g_pszTransBuf = NULL;

    return 0;
}
