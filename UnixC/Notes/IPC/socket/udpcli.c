/*************************************************************************
    > File Name: udpcli.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月27日 星期五 08时17分11秒
    > Description: UDP网络通信(客户端)
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

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
	if (3 > argc) {
		fprintf (stderr, "Usage: %s <Server IP> <port> \n", argv[0]);
		return -1;
	}
	
	printf ("客户机：创建网络数据报套接字...\n");
	
	int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd) {
		perror ("sockfd");
		return -1;
	}
	
	printf ("客户机：准备地址..\n");
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (atoi (argv[2]));
	addr.sin_addr.s_addr = inet_addr (argv[1]);
	
	printf ("客户机：发送请求，并接收响应..\n");
	
	while (1) {
		printf ("%s:%u > ", inet_ntoa (addr.sin_addr), ntohs (addr.sin_port));
		
		char buf[1024];
		gets_s (buf, sizeof (buf));
		
		if (0 == strcmp (buf, "!")){
			break;
		}
		
		if  (-1 == sendto (sockfd, buf, (strlen (buf) + 1) * sizeof (buf[0]),
			0, (struct sockaddr*)&addr, sizeof (addr))) {
			perror ("sendto");
			return -1;
		}
		
		struct sockaddr_in addrsvr = {};
		socklen_t addrlen = sizeof (addrsvr);
		
		ssize_t rb = recvfrom (sockfd, buf, sizeof (buf), 0,
								(struct sockaddr*)&addrsvr, &addrlen);
		if (-1 == rb){
			perror ("recvfrom");
			return -1;
		}
		
		printf ("%s:%u < %s\n", inet_ntoa (addrsvr.sin_addr), ntohs (addrsvr.sin_port),
				buf);
	}
	
	printf ("客户机：关闭套接字..\n");
	
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}
	
	printf ("服务器：一切OK..\n");	
	return 0;
}

