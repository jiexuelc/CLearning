/** @file common.c
 *  @note 
 *  @brief 客户端公共函数
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
#include <sys/stat.h>

#include "common.h"


/* 全局变量定义区 */
TRANS_STATE_E       g_enTransState;                 //传输状态
COM_TRANS_INFO_S    *g_pstComTransInfo  = NULL;     //保存文件信息结构
char                *g_pszTransBuf      = NULL;     //发送或接收缓存
char                g_szAckBuf[ACK_SIZE];           //接收应答缓存
char                *g_pszSha1Digest    = NULL;     //用于存储计算接收后文件的摘要
volatile bool       timeout = false;                //服务器发现计时标志
/* 全局变量定义区 */


 /**@fn     gets_s
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

 /**@fn     IsExist
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


 /**@fn     AddNode
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

/**@fn      FindNode
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

/**@fn      CountNodes
 *  @brief  统计链表内有效节点数
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

 /**@fn     PrintNode
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

  /**@fn     DeleteList
  *  @brief  链表资源释放
  *  @param c 参数描述
  *  @param n 参数描述
  *  @return 返回描述
  */
 void DeleteList(stServerNode *pHead)
 {
    assert(pHead != NULL);

    stServerNode *pTemp = pHead;
 
    while(NULL != pHead)
    {
        pTemp = pHead->pstNext;
        free(pHead);
        pHead = pTemp;
    }
 }


 /**@fn     ProtocolMenu
 *  @brief  协议选项菜单
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void ProtocolMenu(void)
{
    printf("请选择服务协议\n");
    printf("     1-UDP    2-TCP\n");
}

 /**@fn     OperateMenu
 *  @brief  传输方向操作菜单
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
 *  @brief  获取文件大小函数
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 失败返回-1，成功返回文件大小
 */
int GetFileSize(const char* pszFilePath)
{
    struct stat stStat;
    if(0 == stat(pszFilePath, &stStat))
    {
        return stStat.st_size;
    }
    else
    {
        return -1;
    }
}


 /**@fn 
 *  @brief  打印当前工作目录
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
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }

    printf("当前工作目录为:%s\n", pszPath);
    free(pszPath);
}

 /**@fn     PrintDirFile
 *  @brief  打印指定目录文件列表
 *  @param c 参数描述
 *  @param n 参数描述
 *  @return 返回描述
 */
void PrintDirFile(const char* pszDir)
{
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

    printf("该目录文件列表如下:\n");
    printf("*******目录文件列表*******\n");
    for (pstdir = readdir(dp); pstdir; pstdir = readdir(dp))
    {
        /* 不是目录文件直接输出 */
        if (DT_DIR != pstdir->d_type)
        {
            printf ("%s\n", pstdir->d_name);
        }
        /* 不输出. 和..目录文件 */
        else
        {
            if ((0 != strncmp(pstdir->d_name, ".", 1))
                && (0 != strncmp(pstdir->d_name, "..", 2)))
            {
                printf ("./%s/\n", pstdir->d_name);
            }
        }
    }
    printf("*******目录文件列表*******\n");

    closedir(dp);
}
