/** @file udpser.c
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月19日 星期日 11时23分30秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "sha1.h"
#include "udpser.h"
#include "common.h"



 /**@fn 
 *  @brief  服务端UDP服务线程
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
void *UDPService(void *arg)
{
    struct sockaddr_in stClientAddr;
    struct sockaddr_in stServerAddr;
    char szClientAddr[INET_ADDRSTRLEN];
    socklen_t iLenClientAddr = sizeof(stClientAddr);
    int iErrno = -1;
    int iRet = 0;


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    /*初始化地址*/
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stServerAddr.sin_port = htons(MCAST_PORT);

    /*绑定socket*/
    iErrno = bind(sockfd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr));
    if (0 > iErrno)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    /*设置回环许可*/
    int iLoop = 1;
    iErrno = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &iLoop, sizeof(iLoop));
    if (iErrno < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    struct ip_mreq stMreq;/*加入多播组*/
    stMreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);//多播地址
    stMreq.imr_interface.s_addr = htonl(INADDR_ANY); //网络接口为默认

    /*将本机加入多播组*/
    iErrno = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq, sizeof(stMreq));
    if (0 > iErrno)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }
    
    /*循环接收多播组的消息*/
    //while (1)
    {
        memset(&stClientAddr, 0, iLenClientAddr);
        memset(g_pszTransBuf, 0, BUFF_SIZE);

        iRet = recvfrom(sockfd, g_pszTransBuf, BUFF_SIZE, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
        if(-1 == iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return;
        }
        
        /*打印服务器收到的多播信息*/
        printf("buffer: %s\n", g_pszTransBuf);
        if(0 == strncmp(g_pszTransBuf, "build", 5))
        {
            memset(szClientAddr, 0, sizeof(szClientAddr)); 
            inet_ntop(AF_INET, &stClientAddr.sin_addr, szClientAddr, sizeof(szClientAddr));
            printf("client IP: %s Port: %d\n", szClientAddr, ntohs(stClientAddr.sin_port));

            sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
        }
    }

    /*退出多播组, 退出服务器发现状态*/
    iErrno = setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreq, sizeof(stMreq));
 
    /* 循环接收消息 */
    //while (1)
    {

        iRet = recvfrom(sockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
        if(-1 == iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return;
        }
        
        /*打印服务器收到的多播信息*/
        printf("SHA1: %s\n", g_pstComTransInfo->szSHA1);
        printf("FileName: %s\n", g_pstComTransInfo->szFilename);
        printf("enTransFlag: %d\n", g_pstComTransInfo->enTransFlag);
        printf("enTransState: %d\n", g_pstComTransInfo->enTransState);

        sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
    }

    switch (g_pstComTransInfo->enTransState)
    {
        case TRANS_UPLOAD:
        {
            UDPRcvFile(sockfd, &stClientAddr, iLenClientAddr);
            break;
        }
        case TRANS_DOWNLOAD:
        {
            break;
        }
        case TRANS_VIEW_LIST:
        {
            //接收查看目录路径
            //读取文件目录并发送
            break;
        }
        default:
        {
            break;
        }

    }



    close(sockfd);
}

 /**@fn 
 *  @brief  文件接收函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
void UDPRcvFile(int sockfd, struct sockaddr_in *pstClientAddr, socklen_t iLenClientAddr)
{
    fd_set stReadFd;
    struct timeval sttv = {0, 0};
    int ifd;
    int i = 0;  //用于超时计时
    int iRet;   //用于存储返回值
    ifd = open(g_pstComTransInfo->szFilename, O_RDWR | O_CREAT, 0664);
    if(-1 == ifd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    while (1)
    {
        FD_ZERO(&stReadFd);
        FD_SET(sockfd, &stReadFd);
        sttv.tv_sec = 3;
        sttv.tv_usec = 0;
        select(sockfd+1, &stReadFd, NULL, NULL, &sttv);
        
        if(FD_ISSET(sockfd, &stReadFd))
        {
            i = 0;  //超时时间内收到数据重新计数
            iRet = recvfrom(sockfd, (char*)g_pszTransBuf, BUFFER_SIZE, 0, (struct sockaddr*)pstClientAddr, &iLenClientAddr);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            
            if(-1 == write(ifd, g_pszTransBuf, iRet))
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            
            //发送响应
            iRet = sendto(sockfd, "ok", 2, 0, (struct sockaddr*)pstClientAddr, iLenClientAddr);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            //sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)pstClientAddr, iLenClientAddr);
        }
        else
        {
            printf("超时%d次,达到3次本次传输退出!\n", ++i);
            if(i == 3)
            {
                close(ifd);
                break;
            }
        }
    }
    

    SHA1File(g_pstComTransInfo->szFilename, g_pszSha1Digest);
    printf("SHA1: %s\n", g_pszSha1Digest);
    if(0 == strncmp(g_pstComTransInfo->szSHA1, g_pszSha1Digest, 40))
    {
        printf("SHA1相同，文件传输正常\n");
    }
}

 /**@fn 
 *  @brief  文件发送函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
void UDPSendFile(int sockfd, struct sockaddr_in *pstClientAddr, socklen_t iLenClientAddr)
{

}
