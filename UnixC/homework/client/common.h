/** @file common.h
 *  @note 
 *  @brief 客户端公共函数头
 *  
 *  @author 
 *  @date 2019年05月16日 星期四 10时58分18秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#ifndef _COMMON_H
#define _COMMON_H 1

#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define MCAST_PORT  30365
#define TCP_PORT    30366           //TCP服务端口
#define MCAST_ADDR "224.0.0.66"     // 多播地址
#define MCAST_DATA "build"          // 多播内容
#define COM_SHA1DIGEST_LEN  41      //摘要认证字符串最大长度
#define BUFFER_SIZE         1024    //发送或接收数据缓存大小
#define ACK_SIZE            4       //应答缓存大小


#if 0
typedef enum    //传输标志
{
    TRANS_REQ       =  1,  /**< 请求*/
    TRANS_OK        =  2,  /**< 成功*/
    TRANS_ERROR     =  3,  /**< 失败*/
    TRANS_AGAIN     =  4,  /**< 重传*/
    TRANS_ACK       =  5   /**< 确认*/
}TRANS_FLAG_E;
#endif

typedef enum    //传送状态
{
    TRANS_UPLOAD    =  1,  /**< 上传*/
    TRANS_DOWNLOAD  =  2,  /**< 下载*/
    TRANS_VIEW_LIST =  3,  /**< 查看服务端文件列表*/
    TRANS_STAND_BY  =  4   /**< 待就绪*/
}TRANS_STATE_E;

/* 服务器信息 */
typedef struct ServerInfo{ 
   char               *pszIP;
   uint16_t           usiPort;  //16位端口号
   struct ServerInfo  *pstNext;
}stServerNode;

typedef struct tagComTransInfo   //传输文件信息结构
{
    char            szSHA1[COM_SHA1DIGEST_LEN]; //文件摘要信息
    char            szFilename[NAME_MAX];       //文件名
    int             iFileSize;                  //文件大小
    //TRANS_FLAG_E    enTransFlag;                //传输标志
}COM_TRANS_INFO_S;


/* 全局变量声明区 */
extern TRANS_STATE_E       g_enTransState;         //传输状态
extern COM_TRANS_INFO_S    *g_pstComTransInfo;     //保存文件信息结构
extern char                *g_pszTransBuf;         //发送或接收缓存
extern char                g_szAckBuf[ACK_SIZE];   //接收应答缓存
extern char                *g_pszSha1Digest;       //用于存储计算接收后文件的摘要
extern volatile bool       timeout;                //服务器发现计时标志
/* 全局变量声明区 */


char *gets_s(char *str, size_t num, FILE *stream);
bool IsExist(stServerNode *pHead, char* pIP);
void AddNode(stServerNode *pHead, char *pIP, uint16_t usiPort);
stServerNode *FindNode(stServerNode *pHead, uint16_t iNum);
uint16_t CountNodes(stServerNode *pHead);
void PrintNode(stServerNode *pHead);
void DeleteList(stServerNode *pHead);

void ProtocolMenu(void);
void OperateMenu(void);
int GetFileSize(const char* pszFilePath);
void PrintWorkDir(void);
void PrintDirFile(const char* pszDir);

#endif
