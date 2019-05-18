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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#include "common.h"

#define MCAST_PORT 10365
#define MCAST_ADDR "224.0.0.66" /*一个局部连接多播地址，路由器不进行转发*/
#define BUFF_SIZE 256   //接收缓冲区大小


//全局资源申请
void GlobalMemMalloc(void)
{
    //发送或接收缓存
    g_pszTransBuf = (char*)malloc(BUFFER_SIZE);
    if(NULL == g_pszTransBuf)
    {
        printf("内存申请失败！\n");
        return;
    }

    /* 文件信息结构 */
    g_pstComTransInfo = (COM_TRANS_INFO_S*)malloc(sizeof(COM_TRANS_INFO_S));
    if (NULL == g_pstComTransInfo)
    {
        printf("内存申请失败\n");
        return;
    }

    g_pszPath = (char*)malloc(PATH_MAX);
    if (NULL == g_pszPath)
    {
        printf("内存申请失败\n");
        return;
    }

    g_pszSha1Digest = (char*)malloc(COM_SHA1DIGEST_LEN);
    if (NULL == g_pszSha1Digest)
    {
        printf("内存申请失败\n");
        return;
    }

    /* 全局变量内存初始化 */
    memset(g_pszTransBuf, 0, BUFFER_SIZE);
    memset((void*)g_pstComTransInfo, 0, sizeof(COM_TRANS_INFO_S));
    memset(g_pszPath, 0, PATH_MAX);
    memset(g_pszSha1Digest, 0, COM_SHA1DIGEST_LEN);
}

//全局资源释放
void GlobalMemFree(void)
{
    free(g_pszTransBuf);
    g_pstComTransInfo = NULL;

    free(g_pstComTransInfo);
    g_pstComTransInfo = NULL;

    free(g_pszPath);
    g_pszPath = NULL;

    free(g_pszSha1Digest);
    g_pszSha1Digest = NULL;
}

int main()
{
    struct sockaddr_in stClientAddr;
    struct sockaddr_in stServerAddr;
    int err = -1;
    char szClientAddr[INET_ADDRSTRLEN];
    socklen_t iLenClientAddr = sizeof(stClientAddr);
    int iRet = 0;

    GlobalMemMalloc(); //全局资源申请

    memset(&stServerAddr, 0, sizeof(stServerAddr));
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
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
        fprintf(stderr, "%s\n",strerror(errno));
        return -2;
    }

    /*设置回环许可*/
    int iLoop = 1;
    err = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &iLoop, sizeof(iLoop));
    if (err < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return -3;
    }

    struct ip_mreq stMreq;/*加入多播组*/
    stMreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);//多播地址
    stMreq.imr_interface.s_addr = htonl(INADDR_ANY); //网络接口为默认

    /*将本机加入多播组*/
    err = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreq, sizeof(stMreq));
    if (0 > err)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return -4;
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
            return -5;
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

    /*退出多播组*/
    err = setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreq, sizeof(stMreq));
 
    /* 循环接收消息 */
    //while (1)
    {

        iRet = recvfrom(sockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
        if(-1 == iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return -5;
        }
        
        /*打印服务器收到的多播信息*/
        printf("SHA1: %s\n", g_pstComTransInfo->szSHA1);
        printf("FileName: %s\n", g_pstComTransInfo->szFilename);
        printf("enTransFlag: %d\n", g_pstComTransInfo->enTransFlag);
        printf("enTransState: %d\n", g_pstComTransInfo->enTransState);

        sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
    }

    fd_set stReadFd;
    struct timeval sttv = {0, 0};
    int ifd;
    int i = 0;  //用于超时计时
    ifd = open(g_pstComTransInfo->szFilename, O_RDWR | O_CREAT, 0664);
    if(-1 == ifd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
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
            iRet = recvfrom(sockfd, (char*)g_pszTransBuf, BUFFER_SIZE, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return -5;
            }
            
            if(-1 == write(ifd, g_pszTransBuf, iRet))
            {
                fprintf(stderr, "%s\n",strerror(errno));
            }
            
            //发送响应
            iRet = sendto(sockfd, "ok", 2, 0, (struct sockaddr*)&stClientAddr, iLenClientAddr);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return -5;
            }
            //sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
        }
        else
        {
            printf("超时%d次,达到3次本次传输退出!\n", ++i);
            if(i == 3)
            {
                break;
            }
        }
    }
    
    close(ifd);

    SHA1File(g_pstComTransInfo->szFilename, g_pszSha1Digest);
    printf("SHA1: %s\n", g_pszSha1Digest);
    if(0 == strncmp(g_pstComTransInfo->szSHA1, g_pszSha1Digest, 40))
    {
        printf("SHA1相同，文件传输正常\n");
    }

    close(sockfd);
    GlobalMemFree();
    printf("End\n");
    return 0;
}
