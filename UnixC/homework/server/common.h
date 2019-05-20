/** @file common.h
 *  @note 
 *  @brief 公共函数头
 *  
 *  @author jiexue
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
#include <sys/socket.h>
#include <arpa/inet.h>

#define COM_SHA1DIGEST_LEN  41      //摘要认证字符串最大长度
#define BUFFER_SIZE         1024    //发送或接收数据缓存大小
#define ACK_SIZE            4       //应答缓存大小
#define MCAST_PORT  30365           //UDP服务端口
#define TCP_PORT    30366           //TCP服务端口
#define MCAST_ADDR "224.0.0.66"     /*一个局部连接多播地址，路由器不进行转发*/
#define BUFF_SIZE 256               //接收缓冲区大小



typedef enum    //传输标志
{
    TRANS_REQ       =  1,  /**< 请求*/
    TRANS_OK        =  2,  /**< 成功*/
    TRANS_ERROR     =  3,  /**< 失败*/
    TRANS_AGAIN     =  4,  /**< 重传*/
    TRANS_ACK       =  5   /**< 确认*/
}TRANS_FLAG_E;

typedef enum    //传送状态
{
    TRANS_UPLOAD    =  1,  /**< 上传*/
    TRANS_DOWNLOAD  =  2,  /**< 下载*/
    TRANS_VIEW_LIST =  3   /**< 查看服务端文件列表*/
}TRANS_STATE_E;

/* 服务器信息 */
typedef struct ServerInfo{ 
   char     *pszIP;
   uint16_t usiPort; //16位端口号
   struct   ServerInfo *pstNext;
}stServerNode;

typedef struct tagComTransInfo   //传输文件信息结构
{
    char            szSHA1[COM_SHA1DIGEST_LEN]; //文件摘要信息
    char            szFilename[NAME_MAX];       //文件名
    TRANS_FLAG_E    enTransFlag;                //传输标志
}COM_TRANS_INFO_S;

/* 全局变量定义区 */
extern TRANS_STATE_E       g_enTransState;         //传输状态
extern COM_TRANS_INFO_S    *g_pstComTransInfo;     //保存文件信息结构
extern char                *g_pszTransBuf;         //发送或接收缓存
extern char                g_szAckBuf[ACK_SIZE];   //接收应答缓存
extern char                *g_pszPath;             //目录缓存
extern char                *g_pszSha1Digest;       //用于存储计算接收后文件的摘要
/* 全局变量定义区 */

char *gets_s(char *str, size_t num, FILE *stream);
bool IsExist(stServerNode *pHead, char* pIP);
void AddNode(stServerNode *pHead, char *pIP, uint16_t usiPort);
stServerNode *FindNode(stServerNode *pHead, uint16_t iNum);
uint16_t CountNodes(stServerNode *pHead);
void PrintNode(stServerNode *pHead);
void ProtocolMenu(void);
void OperateMenu(void);
void PrintWorkDir(void);
void PrintDirFile(const char* pszDir);
void UDPSendDirList(const char* pszDir, int sockfd, struct sockaddr_in *pstClientAddr, int iLenClientAddr);
void TCPSendDirList(const char* pszDir, int sockfd);

#endif
