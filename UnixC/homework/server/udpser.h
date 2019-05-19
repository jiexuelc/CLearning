/** @file udpser.h
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月19日 星期日 11时23分48秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#ifndef _UDPSER_H
#define _UDPSER_H 1

void *UDPService(void *arg);
void UDPRcvFile(int sockfd, struct sockaddr_in *pstClientAddr, socklen_t iLenClientAddr);
void UDPSendFile(int sockfd, struct sockaddr_in *pstClientAddr, socklen_t iLenClientAddr);

#endif
