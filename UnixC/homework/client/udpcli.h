/** @file udpcli.h
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月17日 星期五 10时16分56秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#ifndef _UDPCLI_H
#define _UDPCLI_H 1

void UDPService(stServerNode *pstServer);
void UDPSendFile(int iSockfd, const char *pszPath, struct sockaddr_in *pstServerAddr);
void UDPRcvFile(int sockfd, struct sockaddr_in *pstServerAddr, socklen_t uliSerAddrLen);

#endif
