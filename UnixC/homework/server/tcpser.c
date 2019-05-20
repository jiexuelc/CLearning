/** @file tcpser.c
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月20日 星期一 19时34分59秒
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

#include "tcpser.h"
#include "sha1.h"
#include "common.h"



 /**@fn 
 *  @brief  服务端TCP服务线程
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
void *TCPService(void *arg)
{
    int iSockfd = *(int *)arg;  //得到线程参数

    int iErrno = -1;
    int iRet = 0;
 
    /* 循环接收消息 */
    while (1)
    {
        printf("等待接收传输状态...\n");
        iRet = recv(iSockfd, (TRANS_STATE_E*)&g_enTransState, sizeof(TRANS_STATE_E), 0);
        if(-1 == iRet)
        {
            fprintf(stderr, "%s\n",strerror(errno));
            return;
        }
        
        /*打印服务器收到的传输标志信息*/
        printf("g_enTransState: %d\n", g_enTransState);

        send(iSockfd, "ok", sizeof("ok"),  0);

        switch (g_enTransState)
        {
            case TRANS_UPLOAD:
            {
                printf("接收文件中...\n");
                iRet = recv(iSockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0);
                if(-1 == iRet)
                {
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }
                else
                {
                    /* 发送本次接收应答 */
                    send(iSockfd, "ok", sizeof("ok"),  0);
                }
                
                /*打印服务器收到的多播信息*/
                printf("SHA1: %s\n", g_pstComTransInfo->szSHA1);
                printf("FileName: %s\n", g_pstComTransInfo->szFilename);
                printf("enTransFlag: %d\n", g_pstComTransInfo->enTransFlag);

                send(iSockfd, "ok", sizeof("ok"),  0);
            
                TCPRcvFile(iSockfd);
                break;
            }
            case TRANS_DOWNLOAD:
            {
                printf("查看本地文件中...\n");
                printf("服务器工作目录%s\n", g_pszPath);
                if(-1 == send(iSockfd, g_pszPath, PATH_MAX,  0))
                {
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }
                else
                {
                    if(-1 == recv(iSockfd, g_szAckBuf, ACK_SIZE, 0))
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
                recv(iSockfd, g_pszPath, PATH_MAX, 0);
                //读取文件目录并发送
                TCPSendDirList(g_pszPath, iSockfd);
                //接收传输文件路径
                recv(iSockfd, g_pszPath, PATH_MAX, 0);

                /* 向客户端传输文件 */
                printf("向客户端传输文件中...\n");
                /* 传输之前获取文件SHA1值 */
                SHA1File(g_pszPath, g_pstComTransInfo->szSHA1);         //存储SHA1摘要
                printf("所选文件SHA1: %s\n", g_pstComTransInfo->szSHA1); 
                char *tmp = strrchr(g_pszPath, '/');
                tmp++;
                snprintf(g_pstComTransInfo->szFilename, NAME_MAX, "%s", tmp);//存储文件名

                /* 发送文件相关信息 */
                if(-1 == send(iSockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0)) 
                {  
                    fprintf(stderr, "%s\n",strerror(errno));
                    return;
                }

                TCPSendFile(iSockfd, g_pszPath);

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

    close(iSockfd);
}

 /**@fn 
 *  @brief  文件接收函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
void TCPRcvFile(int iSockfd)
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
        FD_SET(iSockfd, &stReadFd);
        sttv.tv_sec = 3;
        sttv.tv_usec = 0;
        select(iSockfd+1, &stReadFd, NULL, NULL, &sttv);
        
        if(FD_ISSET(iSockfd, &stReadFd))
        {
            i = 0;  //超时时间内收到数据重新计数
            iRet = recv(iSockfd, (char*)g_pszTransBuf, BUFFER_SIZE, 0);
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
            iRet = send(iSockfd, "ok", 2, 0);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
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
void TCPSendFile(int iSockfd, const char *pszPath)
{
    if (NULL == pszPath)
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
        if((iRet = send(iSockfd, g_pszTransBuf, length, 0)) < 0)
        {            
            printf("Send File:%s Failed.\n", pszPath);
            break; 
        }

        /* 接收本次发送应答 */
        if(recv(iSockfd, g_szAckBuf, ACK_SIZE, 0) < 0)
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
