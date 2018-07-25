/*************************************************************************
    > File Name: netsvr.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月25日 星期三 08时09分05秒
    > Description: 网络通信服务端
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
	if (2 > argc){
		fprintf (stderr, "Usage: %s < port>\n", argv[0]);
		return -1;
	}
	
	printf("服务器：创建网络数据报套接字...\n");
	int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("服务器：准备地址并绑定...\n");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons ( atoi (argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (-1 == bind (sockfd, (struct sockaddr*)&addr, sizeof (addr))) {
		perror ("bind");
		return -1;
	}	

	printf("服务器：接收数据...\n");
	while (1) {
		char buf[1024];
		ssize_t rb = read (sockfd, buf, sizeof (buf));
		if (-1 == rb) {
			perror("read");
			return -1;
		}
		//接收到关闭服务端命令，关闭服务器端
		if (0 == strcmp (buf,"!!")) {
			break;
		}

		printf ("< %s\n", buf);
	}

	printf("服务器：工作关闭套接字...\n");
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}

	printf("服务器：一切OK...\n");

	return 0;
}

