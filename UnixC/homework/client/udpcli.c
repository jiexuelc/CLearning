/** @file udpcli.c
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月17日 星期五 10时16分50秒
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
#include "udpcli.h"
#include "sha1.h"


 /**@fn 
 *  @brief  UDP传输文件服务
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void UDPService(stServerNode *pstServer)
{
    assert(pstServer != NULL);

    struct sockaddr_in stServerAddr;
    int iSockfd;
    int iRet = 0;
    char cBuf;//临时变量，清输入缓存用
    char *pszPath = (char*)malloc(PATH_MAX);    //装载目录
    if(NULL == pszPath)
    {
        printf("内存申请失败！\n");
        return;
    }
    
    if ((iSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }
    
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_addr.s_addr = inet_addr(pstServer->pszIP);
    stServerAddr.sin_port = htons(pstServer->usiPort);
    
    OperateMenu();//操作选择界面

    uint16_t usiNum;

    scanf("%hu", &usiNum);
    while (((cBuf = getchar()) != EOF) && (cBuf != '\n'));//使用getchar()获取输入缓冲区字符，直到获取的c是'\n'或文件结尾符EOF为止

    /* 上传操作 */
    if (1 == usiNum)
    {
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
        SHA1File(g_pstComTransInfo->szFilename, g_pstComTransInfo->szSHA1);
        printf("所选文件SHA1: %s\n", g_pstComTransInfo->szSHA1);

        g_pstComTransInfo->enTransState = TRANS_UPLOAD;   //置为上传状态
        g_pstComTransInfo->enTransFlag = TRANS_REQ;

        /* 发送文件相关信息 */
        iRet = sendto(iSockfd, (COM_TRANS_INFO_S*)g_pstComTransInfo, sizeof(COM_TRANS_INFO_S), 0, (struct sockaddr *)&stServerAddr, sizeof(stServerAddr));
        if(-1 == iRet) 
        {  
            fprintf(stderr, "%s\n",strerror(errno));
            return;
        }
        else
        {
            printf("发送%d字节消息\n", iRet);
        }

        UDPSendFile(iSockfd, pszPath, &stServerAddr);

        #if 0
        printf("睡眠7s继续发送数据\n");
        sleep(7);
        UDPSendFile(iSockfd, pszPath, &stServerAddr);
        #endif

        free(pszPath);
    }
    /* 下载操作 */
    else
    {
        //查看远端服务器文件
        //选择下载文件
        //接收文件内容
        ;
    }
}

 /**@fn 
 *  @brief  UDP发送文件服务
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void UDPSendFile(int iSockfd, const char *pszPath, struct sockaddr_in *pstServerAddr)
{
    if((NULL == pszPath) || (NULL == pstServerAddr))
    {
        printf("传入参数错误！\n");
        return;
    }

    int length = 0;
    int iRet = 0;
    socklen_t uliAddrLen = sizeof(struct sockaddr_in);
    int ifd = open(pszPath, O_RDONLY);
    if (-1 == ifd)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    while ((length = read(ifd, g_pszTransBuf, BUFFER_SIZE)) > 0)
    {
        //printf("读取%d字节消息\n", length);   //发送
        if((iRet = (sendto(iSockfd, g_pszTransBuf, length, 0, (struct sockaddr*)pstServerAddr, sizeof(struct sockaddr)))) < 0)          
        {            
            printf("Send File:%s Failed.\n", pszPath);
            break; 
        }

        /* 接收本次发送应答 */
        if((recvfrom(iSockfd, szAckBuf, ACK_SIZE, 0, (struct sockaddr*)pstServerAddr, &uliAddrLen)) < 0)          
        {            
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        }
        if(0 == strncmp(szAckBuf, "ok", 2))
        {
            printf("发送%d字节消息成功\n", iRet);
        }
    }

    //sendto(iSockfd, "ok", 2, 0, (struct sockaddr*)pstServerAddr, sizeof(struct sockaddr));      
    printf("File:%s Transfer Successful!\n", pszPath); 

    close(ifd);
}

 /**@fn 
 *  @brief  UDP接收文件服务
 *  @param c 参数描述.
 *  @param n 参数描述.
 *  @return 返回描述
 */
void UDPRcvFile(int iSockfd, const char *pszPath, struct sockaddr_in *pstServerAddr)
{

}
