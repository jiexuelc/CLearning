/*************************************************************************
    > File Name: udpsvr.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月26日 星期四 20时45分09秒
    > Description: UDP网络通信(服务端)
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) 
{
	if (2 > argc) {
		fprintf (stderr, "Usage: %s <port> \n", argv[0]);
		return -1;
	}
	
	printf ("服务器：创建网络数据报套接字...\n");
	
	int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("sockfd");
		return -1;
	}
	
	printf ("服务器：准备地址并绑定..\n");
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (atoi (argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (-1 == bind (sockfd, (struct sockaddr*)&addr, sizeof (addr))) {
		perror ("bind");
		return -1;
	}
	
	while (1) {
		printf ("服务器：接收请求..\n");
		
		char buf[1024];
		struct sockaddr_in addrcli = {};
		socklen_t addrlen = sizeof (addrcli);
		
		ssize_t rb = recvfrom (sockfd, buf, sizeof (buf), 0,
								(struct sockaddr*)&addrcli, &addrlen);
		if (-1 == rb){
			perror ("recvfrom");
			return -1;
		}
		
		printf ("服务器：向%s:%u客户机发送响应字符串> %s\n", inet_ntoa (addrcli.sin_addr),
				ntohs (addrcli.sin_port), buf);
		if  (-1 == sendto (sockfd, buf, rb, 0, (struct sockaddr*)&addrcli, addrlen)) {
			perror ("sendto");
			return -1;
		}
	}
	
	printf ("服务器：关闭套接字..\n");
	
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}
	
	printf ("服务器：一切OK..\n");	
	return 0;
}
