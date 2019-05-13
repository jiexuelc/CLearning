/** @file server.c
 *  @note 
 *  @brief 说明本文件
 *  
 *  @author jiexue
 *  @date 2019年05月13日 星期一 16时12分50秒
 *  
 *  @note 
 *  
 *  @warning 
 */

#define COM_PORT 9988

#include <stdio.h>
#include <netinet/in.h>

int main()
{
    printf("创建网络数据流套接字。。。\n");
    int iSockFd = soket(AF_INET, SOCK_STREAM, 0);
    if(-1 == iSockFd)
    {
        perror("socket");
        return -1;
    }

    printf("准备地址。。。\n");
    struct socketaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(COM_PORT);
    addr.sin_addr = INADDR_ANY;

    if(-1 == bind(iSockFd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        perror("bind");
        return -1;
    }

    if(-1 == listen(iSockFd, 16))
    {
        perror("listen");
        return -1;
    }

    while (1)
    {
        printf("等待连接。。。\n");

        struct sockaddr_in addr
    }
}
