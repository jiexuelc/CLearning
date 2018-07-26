/*************************************************************************
    > File Name: tcpcli.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月26日 星期四 08时21分39秒
    > Description: TCP网络通信(客户端)
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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


int main(int argc, char* argv[])
{
	if (3 > argc){
		fprintf (stderr, "Usage: %s <Server IP> < port>\n", argv[0]);
		return -1;
	}
	
	printf("客户机：创建网络数据报套接字...\n");
	int sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("客户机：准备地址并连接...\n");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (atoi (argv[2]));
	//addr.sin_port = htonl (atoi (argv[2]));
	addr.sin_addr.s_addr = inet_addr (argv[1]);
	
	if (-1 == connect (sockfd, (struct sockaddr*)&addr, sizeof (addr))) {
		perror ("connect");
		return -1;
	}

	printf("客户机：发送请求并接收响应...\n");

	while(1) {
		printf ("> ");
		
		char buf[1024];
		gets_s (buf, sizeof (buf));
		
		if (0 == strcmp (buf, "!")) {
			break;
		}
		
		if (-1 == send (sockfd, buf, (strlen (buf) +1 ) *
			sizeof (buf[0]), 0)) {
			perror ("send");
			return -1;
			}
		
		ssize_t rb = recv ( sockfd, buf, sizeof (buf), 0);
		if (-1 == rb) {
			perror ("recv");
			return -1;
		}
		if (0 == rb) {
			printf ("客户机：服务器已经宕机！\n");
			break;
		}
		
		printf ("< %s\n", buf);
	}

	printf("客户机：关闭套接字...\n");
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}

	printf("客户机：一切OK...\n");

	return 0;
}


