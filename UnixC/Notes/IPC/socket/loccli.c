/*************************************************************************
    > File Name: loccli.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月24日 星期二 08时12分23秒
    > Description: 本地套接字(用于进程通信)客户端
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCK_FILE "sock"

char *gets_s(char *str, size_t num)
{
    if (0 != fgets(str, num, stdin))
    {
        size_t len = strlen(str);
        if (len > 0 && str[len-1] == '\n')
            str[len-1] = '\0';
        return str;
    }
    return 0;
}

int main()
{
	printf("客户机：创建本地套接字...\n");
	int sockfd = socket (AF_LOCAL, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("客户机：准备地址并连接...\n");
	struct sockaddr_un addr;
	addr.sun_family = AF_LOCAL;
	strcpy (addr.sun_path, SOCK_FILE);
	if (-1 == connect (sockfd, (struct sockaddr*)&addr, sizeof (addr))) {
		perror ("connect");
		return -1;
	}
	

	printf("客户机：发送数据...\n");
	while (1) {
		char buf[1024];
		printf ("> ");
		gets_s (buf, sizeof (buf));
		
		//关闭客户机
		if (0 == strcmp (buf,"!")) {
			break;
		}
		
		ssize_t rb = write (sockfd, buf, (strlen (buf) + 1) * sizeof (buf[0]));
		if (-1 == rb) {
			perror("write");
			return -1;
		}
		
		//关闭服务器端，并关闭客户机
		if (0 == strcmp (buf,"!!")) {
			break;
		}
	}

	printf("客户机：关闭套接字...\n");
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}

	printf("客户机：一切OK...\n");

	return 0;
}
