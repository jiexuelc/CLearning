/*************************************************************************
    > File Name: locsvr.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月24日 星期二 08时12分23秒
    > Description: 本地套接字(用于进程通信)服务端
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCK_FILE "sock"

int main()
{
	printf("服务器：创建本地套接字...\n");
	int sockfd = socket (AF_LOCAL, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("服务器：准备地址并绑定...\n");
	struct sockaddr_un addr;
	addr.sun_family = AF_LOCAL;
	strcpy (addr.sun_path, SOCK_FILE);
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

	printf("服务器：删除套接字文件...\n");
	if (-1 == unlink (SOCK_FILE)) {
		perror ("unlink");
		return -1;
	}
	return 0;
}
