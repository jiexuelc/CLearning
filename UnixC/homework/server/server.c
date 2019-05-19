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
#include "sha1.h"
#include "udpser.h"


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
    char szClientAddr[INET_ADDRSTRLEN];
    socklen_t iLenClientAddr = sizeof(stClientAddr);

    GlobalMemMalloc(); //全局资源申请

    memset(&stServerAddr, 0, sizeof(stServerAddr));
    getcwd(g_pszPath, PATH_MAX);    //获取当前工作目录
    printf("Working Path: %s\n", g_pszPath);

    /* 创建UDP服务线程 */
    pthread_t UDPtid;
    if(-1 == pthread_create(&UDPtid, NULL, UDPService, NULL))
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return -1;
    }
    pthread_detach(UDPtid);   //分离线程
    

    while (1)
    {
        sleep(1);
    }
    
    GlobalMemFree();
    printf("End\n");
    return 0;
}
