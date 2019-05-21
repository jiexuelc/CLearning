/** @file tcpcli.h
 *  @note 
 *  @brief 客户端tcp服务实现头文件
 *  
 *  @author 
 *  @date 2019年05月16日 星期四 21时01分38秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#ifndef _TCPCLI_H
#define _TCPCLI_H 1

void TCPService(stServerNode *pstServer);
void TCPSendFile(int iSockfd, char *pszPath);
void TCPRcvFile(int sockfd);

#endif
