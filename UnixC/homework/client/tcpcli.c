/** @file tcpcli.c
 *  @note 
 *  @brief 客户端tcp服务实现
 *  
 *  @author 
 *  @date 2019年05月16日 星期四 21时00分46秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include "common.h"
#include "tcpcli.h"
#include "sha1.h"


 /**@fn 
 *  @brief  TCP传输文件服务
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void TCPService(stServerNode *pstServer)
{
    assert(pstServer != NULL);

    struct sockaddr_in stServerAddr;
    socklen_t uliSerAddrLen = sizeof(stServerAddr);
    int iSockfd;
    int iRet = 0;
    char cBuf;//临时变量，清输入缓存用
    char *pszPath = (char*)malloc(PATH_MAX);    //装载目录
    if(NULL == pszPath)
    {
        printf("内存申请失败！\n");
        return;
    }
    
    if ((iSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }
    
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_addr.s_addr = inet_addr(pstServer->pszIP);
    stServerAddr.sin_port = htons(TCP_PORT);
    if(-1 == connect(iSockfd, (struct sockaddr *) &stServerAddr, sizeof(stServerAddr)))
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    uint16_t usiNum;

    while (1)
    {
        OperateMenu();//操作选择界面
        scanf("%hu", &usiNum);
        while (((cBuf = getchar()) != EOF) && (cBuf != '\n'));//使用getchar()获取输入缓冲区字符，直到获取的c是'\n'或文件结尾符EOF为止

        /* 上传操作 */
        if (1 == usiNum)
        {
            usiNum = 0;
            g_enTransState = TRANS_UPLOAD;   //置为上传状态
            iRet = send(iSockfd, (TRANS_STATE_E*)&g_enTransState, sizeof(TRANS_STATE_E), 0);
            if(-1 == iRet) 
            {  
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            else
            {
                /* 接收本次发送应答 */
                if((recv(iSockfd, g_szAckBuf, ACK_SIZE, 0)) < 0)
                {            
                    fprintf(stderr, "%s\n", strerror(errno));
                    return;
                }
                if(0 == strncmp(g_szAckBuf, "ok", 2))
                {
                    printf("服务器已准备好接收文件\n");
                }
            }

            PrintWorkDir(); //打印当前工作目录
            printf("请输入需要查看的目录绝对路径: ");
            gets_s(pszPath, PATH_MAX, stdin);  //获取输入目录
            PrintDirFile(pszPath);  //输出输入目录下的文件及目录
            
            printf("输入需要上传的文件名:\n");
            gets_s(g_pstComTransInfo->szFilename, NAME_MAX, stdin);   //获取上传文件名
            strncat(pszPath, "/", 1);
            strncat(pszPath, g_pstComTransInfo->szFilename, sizeof(g_pstComTransInfo->szFilename));
            printf("文件绝对路径: %s\n", pszPath);

            /* 上传之前获取文件SHA1值 */
            SHA1File(pszPath, g_pstComTransInfo->szSHA1);
            printf("待上传文件SHA1:      %s\n", g_pstComTransInfo->szSHA1);
            /* 获取文件大小 */
            g_pstComTransInfo->iFileSize = GetFileSize(pszPath);
            printf("待上传文件大小: %d\n", g_pstComTransInfo->iFileSize);
            
            //g_pstComTransInfo->enTransFlag = TRANS_REQ;         //置为上传请求

            /* 发送文件相关信息 */
            iRet = send(iSockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0);
            if(-1 == iRet) 
            {  
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            else
            {
                /* 接收本次发送应答 */
                if((recv(iSockfd, g_szAckBuf, ACK_SIZE, 0)) < 0)
                {            
                    fprintf(stderr, "%s\n", strerror(errno));
                    return;
                }
                if(0 == strncmp(g_szAckBuf, "ok", 2))
                {
                    printf("服务器已准备接收文件\n");
                }
                printf("服务器已接收到%d字节文件相关信息\n", iRet);
            }

            TCPSendFile(iSockfd, pszPath);

            #if 0
            printf("睡眠7s继续发送数据\n");
            sleep(7);
            TCPSendFile(iSockfd, pszPath);
            #endif
        }
        /* 下载操作 */
        else
        {
            usiNum = 0;
            //发送查看服务器目录命令
            g_enTransState = TRANS_DOWNLOAD;   //置为下载状态
            iRet = send(iSockfd, (TRANS_STATE_E*)&g_enTransState, sizeof(TRANS_STATE_E), 0);
            if(-1 == iRet) 
            {  
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }
            else
            {
                /* 接收本次发送应答 */
                if((recv(iSockfd, g_szAckBuf, ACK_SIZE, 0)) < 0)          
                {            
                    fprintf(stderr, "%s\n", strerror(errno));
                    return;
                }
                if(0 == strncmp(g_szAckBuf, "ok1", 3))
                {
                    printf("服务器已准备好查看\n");
                }
            }

            //接收当前工作目录
            if(recv(iSockfd, pszPath, PATH_MAX, 0) < 0)
            {            
                fprintf(stderr, "%s\n", strerror(errno));
                return;
            }
            else
            {
                printf("服务器当前工作目录: %s\n", pszPath);
                if(-1 == send(iSockfd, "ok", 2, 0))
                {
                    fprintf(stderr, "%s\n", strerror(errno));
                    return;
                }
            }

            /* 查看远端服务器文件 */
            printf("请输入需要查看的目录绝对路径: ");
            gets_s(pszPath, PATH_MAX, stdin);  //获取输入目录
            send(iSockfd, pszPath, PATH_MAX, 0);
            printf("服务器该目录文件列表如下:\n");
            printf("*******目录文件列表*******\n");
            while(1)
            {
                if(recv(iSockfd, g_pstComTransInfo->szFilename, NAME_MAX, 0) < 0)
                {            
                    fprintf(stderr, "%s\n", strerror(errno));
                    return;
                }
                else
                {
                    if(0 == strncmp(g_pstComTransInfo->szFilename, "**", 2))
                    {
                        break;
                    }
                    printf("%s\n", g_pstComTransInfo->szFilename);
                }
            }
            printf("*******目录文件列表*******\n");
            
            /* 选择服务器文件 */
            printf("输入需要下载的文件名:\n");
            gets_s(g_pstComTransInfo->szFilename, NAME_MAX, stdin);   //获取下载文件名
            strncat(pszPath, "/", 1);
            strncat(pszPath, g_pstComTransInfo->szFilename, sizeof(g_pstComTransInfo->szFilename));
            printf("文件绝对路径: %s\n", pszPath);
            send(iSockfd, pszPath, PATH_MAX, 0); //将待下载文件绝对路径发给服务器

            /* 接收文件相关信息 */
            if(-1 == recv(iSockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0))
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }

            printf("SHA1: %s\n", g_pstComTransInfo->szSHA1);
            printf("FileName: %s\n", g_pstComTransInfo->szFilename);
            printf("FileSize: %d\n", g_pstComTransInfo->iFileSize);
            //printf("enTransFlag: %d\n", g_pstComTransInfo->enTransFlag);

            //接收文件内容
            TCPRcvFile(iSockfd);
        }
    }

    free(pszPath);
    close (iSockfd);
}

 /**@fn 
 *  @brief  TCP发送函数
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void TCPSendFile(int iSockfd, char *pszPath)
{
    if(NULL == pszPath)
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
            printf("%s文件发送失败!\n", pszPath);
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
            ;
            //printf("发送%d字节消息成功\n", iRet);
        }
    }
   
    printf("%s文件发送成功!\n", pszPath); 

    close(ifd);
}

 
 /**@fn 
 *  @brief  TCP接收函数
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void TCPRcvFile(int sockfd)
{
    fd_set stReadFd;
    //struct timeval sttv = {0, 0};
    int ifd;
    int i = 0;  //用于超时计时
    int iRet;   //用于存储返回值
    int iRevSize = 0;   //用于接收文件大小计数
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
        //sttv.tv_sec = 3;
        //sttv.tv_usec = 0;
        //select(sockfd+1, &stReadFd, NULL, NULL, &sttv);
        select(sockfd+1, &stReadFd, NULL, NULL, NULL);
        
        if(FD_ISSET(sockfd, &stReadFd))
        {
            i = 0;  //超时时间内收到数据重新计数
            iRet = recv(sockfd, (char*)g_pszTransBuf, BUFFER_SIZE, 0);
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

            iRevSize += iRet;
            
            //发送响应
            iRet = send(sockfd, "ok", 2, 0);
            if(-1 == iRet)
            {
                fprintf(stderr, "%s\n",strerror(errno));
                return;
            }

            //文件接收计数达到文件大小则接收结束
            if(iRevSize == g_pstComTransInfo->iFileSize)
            {
                break;
            }
        }
        else
        {
            printf("超时%d次,达到3次本次传输退出!\n", ++i);
            if(i == 3)
            {
                printf("网络异常，文件传输未完成!\n");
                close(ifd);
                break;
            }
        }
    }
    

    SHA1File(g_pstComTransInfo->szFilename, g_pszSha1Digest);
    printf("SHA1: %s\n", g_pszSha1Digest);
    if(0 == strncmp(g_pstComTransInfo->szSHA1, g_pszSha1Digest, 40))
    {
        printf("SHA1相同，文件下载正常\n");
    }
}
