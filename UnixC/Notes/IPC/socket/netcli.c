/*************************************************************************
    > File Name: netcli.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月25日 星期三 08时09分24秒
    > Description: 网络通信客户端
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


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
		fprintf (stderr, "Usage: %s <server IP> < port>\n", argv[0]);
		return -1;
	}
	
	printf("客户机：创建本地套接字...\n");
	int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("客户机：准备地址并连接...\n");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (atoi (argv[2]));
	addr.sin_addr.s_addr = inet_addr (argv[1]);
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
