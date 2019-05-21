/** @file common.c
 *  @note 
 *  @brief 服务端公共函数
 *  
 *  @author 
 *  @date 2019年05月16日 星期四 10时58分09秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "common.h"


/* 全局变量定义区 */
TRANS_STATE_E       g_enTransState;                 //传输状态
COM_TRANS_INFO_S    *g_pstComTransInfo  = NULL;     //保存文件信息结构
char                *g_pszTransBuf      = NULL;     //发送或接收缓存
char                g_szAckBuf[ACK_SIZE];           //接收应答缓存
char                *g_pszPath          = NULL;     //目录缓存
char                *g_pszSha1Digest    = NULL;     //用于存储计算接收后文件的摘要
/* 全局变量定义区 */


 /**@fn 
 *  @brief  带'\0'且丢掉'\n'的字符串获取函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 成功返回字符串地址，失败返回空
 */
char *gets_s(char *str, size_t num, FILE *stream)
{
    if (0 != fgets(str, num, stream))
    {
        size_t len = strlen(str);
        if (len > 0 && str[len-1] == '\n')
            str[len-1] = '\0';
        return str;
    }
    return 0;
}

 /**@fn 
 *  @brief  检测链表中是否存在该IP信息
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
bool IsExist(stServerNode *pHead, char* pIP)
{
    assert(pHead != NULL);

    stServerNode *pTemp = pHead->pstNext;

    while(pTemp != NULL)
    {
        if(1 == strncmp(pTemp->pszIP, pIP, sizeof(pIP)))
        {
            return true;
        }
        pTemp = pTemp->pstNext;
    }

    return false;
}


 /**@fn 
 *  @brief  将新节点插入头指针后
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */ 
void AddNode(stServerNode *pHead, char *pIP, uint16_t usiPort)
{
    assert(pHead != NULL);

    stServerNode *pTemp = pHead->pstNext;//保存原链表第一个节点指针

    stServerNode *pNode = (stServerNode *)malloc(sizeof(stServerNode));
    if(NULL == pNode)
    {
        printf("内存申请失败\n");
        return;
    }

    memset(pNode, 0, sizeof(stServerNode));
    
    pNode->pszIP = (char*)malloc(strlen(pIP)+1);
    if(NULL == pNode->pszIP)
    {
        printf("内存申请失败\n");
        return;
    }

    memset(pNode->pszIP, 0, strlen(pIP) + 1);
    strncpy(pNode->pszIP, pIP, strlen(pIP));
    pNode->usiPort = usiPort;
    pNode->pstNext = pTemp;//插入节点内的指针指向原链表的第一个节点
    pHead->pstNext = pNode;//将头节点指向插入节点
}

/**@fn 
 *  @brief  查找指定序号IP服务器信息
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
stServerNode *FindNode(stServerNode *pHead, uint16_t iNum)
{
    assert(pHead != NULL);

    stServerNode *pTemp = pHead->pstNext;

    while(--iNum)
    {
        pTemp = pTemp->pstNext;
    }

    return pTemp;
}

/**@fn 
 *  @brief  查找链表内有效节点数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
uint16_t CountNodes(stServerNode *pHead)
{
    assert(pHead != NULL);

    stServerNode *pTemp = pHead->pstNext;
    uint16_t usiNum = 0;

    while(pTemp)
    {
        ++usiNum;
        pTemp = pTemp->pstNext;
    }

    return usiNum;
}

 /**@fn 
 *  @brief  打印链表中所有的IP服务器信息
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void PrintNode(stServerNode *pHead)
{
    assert(pHead != NULL);

    stServerNode *pTemp = pHead->pstNext;
    int iNum = 0;

    while(pTemp != NULL)
    {
        iNum++;
        printf("服务器%3d [%s:%hu]\n", iNum, pTemp->pszIP, pTemp->usiPort);
        pTemp = pTemp->pstNext;
    }
}

 /**@fn 
 *  @brief  协议选项函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void ProtocolMenu(void)
{
    printf("请选择服务协议\n");
    printf("     1-UDP    2-TCP\n");
}

 /**@fn 
 *  @brief  操作菜单显示
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void OperateMenu(void)
{
    printf("\n");

    printf("\t1-uploadfile(上传)\n");
    printf("\t2-downloadfile(下载)\n");
    
    printf("\n请选择需要进行的操作:\n");
}

 /**@fn 
 *  @brief  打印当前目录
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void PrintWorkDir(void)
{
    //printf("%d\n", PATH_MAX);
    char *pszPath = (char*)malloc(PATH_MAX);    //存储目录变量
    if(NULL == pszPath)
    {
        printf("内存申请失败\n");
        return;
    }

    if(NULL == getcwd(pszPath, PATH_MAX))   //获取当前目录
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    printf("当前工作目录为:%s\n", pszPath);
    free(pszPath);
}

 /**@fn 
 *  @brief  发送指定目录文件列表
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void UDPSendDirList(const char* pszDir, int sockfd, struct sockaddr_in *pstClientAddr, int iLenClientAddr)
{
    char szFileName[NAME_MAX];
    if(NULL == pszDir)
    {
        printf("请检查，传入参数错误！\n");
        return;
    }

    struct dirent* pstdir;
    DIR *dp = opendir(pszDir);
    if (NULL == dp)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    //printf("该目录文件列表如下:\n");
    //printf("*******目录文件列表*******\n");
    for (pstdir = readdir(dp); pstdir; pstdir = readdir(dp))
    {
        /* 不是目录文件直接输出 */
        if (DT_DIR != pstdir->d_type)
        {
            sprintf(szFileName, "%s", pstdir->d_name);
        }
        /* 不输出. 和..目录文件 */
        else
        {
            if ((0 != strncmp(pstdir->d_name, ".", 1))
                && (0 != strncmp(pstdir->d_name, "..", 2)))
            {
                sprintf (szFileName, "./%s", pstdir->d_name);
            }
        }
        //printf ("%s\n", szFileName);
        sendto(sockfd, szFileName, NAME_MAX, 0, (struct sockaddr*)pstClientAddr, iLenClientAddr);
    }
    //printf("*******目录文件列表*******\n");
    sendto(sockfd, "**", 2, 0, (struct sockaddr*)pstClientAddr, iLenClientAddr);    //文件目录传输结束标志

    closedir(dp);
}

 /**@fn 
 *  @brief  发送指定目录文件列表
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void TCPSendDirList(const char* pszDir, int sockfd)
{
    char szFileName[NAME_MAX];
    if(NULL == pszDir)
    {
        printf("请检查，传入参数错误！\n");
        return;
    }

    struct dirent* pstdir;
    DIR *dp = opendir(pszDir);
    if (NULL == dp)
    {
        fprintf(stderr, "%s\n",strerror(errno));
        return;
    }

    //printf("该目录文件列表如下:\n");
    //printf("*******目录文件列表*******\n");
    for (pstdir = readdir(dp); pstdir; pstdir = readdir(dp))
    {
        /* 不是目录文件直接输出 */
        if (DT_DIR != pstdir->d_type)
        {
            sprintf(szFileName, "%s", pstdir->d_name);
        }
        /* 不输出. 和..目录文件 */
        else
        {
            if ((0 != strncmp(pstdir->d_name, ".", 1))
                && (0 != strncmp(pstdir->d_name, "..", 2)))
            {
                sprintf (szFileName, "./%s", pstdir->d_name);
            }
        }
        //printf ("%s\n", szFileName);
        send(sockfd, szFileName, NAME_MAX, 0);
    }
    //printf("*******目录文件列表*******\n");
    send(sockfd, "**", 2, 0);    //文件目录传输结束标志

    closedir(dp);
}
