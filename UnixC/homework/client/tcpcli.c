/** @file tcpcli.c
 *  @note 
 *  @brief tcp服务实现
 *  
 *  @author jiexue
 *  @date 2019年05月16日 星期四 21时00分46秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "common.h"
#include "tcpcli.h"
#include "sha1.h"

void TCPService(stServerNode *pstServer)
{
    assert(pstServer != NULL);

    struct sockaddr_in stServerAddr;
    int iSockfd;
    
    if ((iSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_addr.s_addr = inet_addr(pstServer->pszIP);
    stServerAddr.sin_port = htons(pstServer->usiPort);
    
    OperateMenu();
}
