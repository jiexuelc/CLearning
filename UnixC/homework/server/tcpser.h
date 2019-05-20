/** @file tcpser.h
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月20日 星期一 19时34分53秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#ifndef _TCPSER_H
#define _TCPSER_H 1

void *TCPService(void *arg);
void TCPRcvFile(int iSockfd);
void TCPSendFile(int iSockfd, const char *pszPath);

#endif
