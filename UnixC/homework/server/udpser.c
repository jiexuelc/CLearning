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
    while (1)
    {
        printf("等待接收传输状态...\n");
        iRet = recvfrom(sockfd, (TRANS_STATE_E*)&g_enTransState, sizeof(TRANS_STATE_E), 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
        if(-1 == iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return;
        }
        
        /*打印服务器收到的传输标志信息*/
        printf("g_enTransState: %d\n", g_enTransState);

        sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);

        switch (g_enTransState)
        {
            case TRANS_UPLOAD:
            {
                printf("接收文件中...\n");
                iRet = recvfrom(sockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
                if(-1 == iRet)
                {
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }
                else
                {
                    /* 发送本次接收应答 */
                    sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
                }
                
                /*打印服务器收到的多播信息*/
                printf("SHA1: %s\n", g_pstComTransInfo->szSHA1);
                printf("FileName: %s\n", g_pstComTransInfo->szFilename);
                printf("enTransFlag: %d\n", g_pstComTransInfo->enTransFlag);

                sendto(sockfd, "ok", sizeof("ok"),  0, (struct sockaddr *)&stClientAddr, iLenClientAddr);
            
                UDPRcvFile(sockfd, &stClientAddr, iLenClientAddr);
                break;
            }
            case TRANS_DOWNLOAD:
            {
                printf("查看本地文件中...\n");
                printf("服务器工作目录%s\n", g_pszPath);
                if(-1 == sendto(sockfd, g_pszPath, PATH_MAX,  0, (struct sockaddr *)&stClientAddr, iLenClientAddr))
                {
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }
                else
                {
                    if(-1 == recvfrom(sockfd, g_szAckBuf, ACK_SIZE, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr))
                    {
                        fprintf(stderr, "%s\n",strerror(errno));
                        return;
                    }
                    else
                    {
                        strncmp(g_szAckBuf, "ok", 2);
                        printf("本地目录发送成功！\n");
                    }
                }

                //接收查看目录路径
                recvfrom(sockfd, g_pszPath, PATH_MAX, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);
                //读取文件目录并发送
                SendDirList(g_pszPath, sockfd, &stClientAddr, iLenClientAddr);
                //接收传输文件路径
                recvfrom(sockfd, g_pszPath, PATH_MAX, 0, (struct sockaddr*)&stClientAddr, &iLenClientAddr);

                /* 向客户端传输文件 */
                printf("向客户端传输文件中...\n");
                /* 传输之前获取文件SHA1值 */
                SHA1File(g_pszPath, g_pstComTransInfo->szSHA1);         //存储SHA1摘要
                printf("所选文件SHA1: %s\n", g_pstComTransInfo->szSHA1); 
                char *tmp = strrchr(g_pszPath, '/');
                tmp++;
                snprintf(g_pstComTransInfo->szFilename, NAME_MAX, "%s", tmp);//存储文件名

                /* 发送文件相关信息 */
                if(-1 == sendto(sockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0, (struct sockaddr *)&stClientAddr, iLenClientAddr)) 
                {  
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }

                UDPSendFile(sockfd, &stClientAddr, iLenClientAddr, g_pszPath);

                break;
            }
            case TRANS_VIEW_LIST:
            {
                break;
            }
            default:
            {
                break;
            }

        }

        #if 0
        if(TRANS_VIEW_LIST != g_enTransState)
        {
            printf("下载或上传文件结束...\n");
            break;
        }
        #endif
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
void UDPSendFile(int sockfd, struct sockaddr_in *pstClientAddr, socklen_t iLenClientAddr, const char *pszPath)
{
    if((NULL == pszPath) || (NULL == pstClientAddr))
    {
        printf("传入参数错误！\n");
        return;
    }

    int length = 0;
    int iRet = 0;
    int ifd = open(pszPath, O_RDONLY);
    if (-1 == ifd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    while ((length = read(ifd, g_pszTransBuf, BUFFER_SIZE)) > 0)
    {
        //printf("读取%d字节消息\n", length);   //发送
        if((iRet = (sendto(sockfd, g_pszTransBuf, length, 0, (struct sockaddr*)pstClientAddr, sizeof(struct sockaddr)))) < 0)          
        {            
            printf("Send File:%s Failed.\n", pszPath);
            break; 
        }

        /* 接收本次发送应答 */
        if((recvfrom(sockfd, g_szAckBuf, ACK_SIZE, 0, (struct sockaddr*)pstClientAddr, &iLenClientAddr)) < 0)          
        {            
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        }
        if(0 == strncmp(g_szAckBuf, "ok", 2))
        {
            printf("发送%d字节消息成功\n", iRet);
        }
    }
     
    printf("File:%s Transfer Successful!\n", pszPath); 

    close(ifd);
}



