/*************************************************************************
    > File Name: tcpsvr.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月25日 星期三 20时06分25秒
    > Description: TCP网络通信(服务端)
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void sigchld (int signum) {
	while (1) {
		pid_t pid = waitpid (-1, 0,WNOHANG);
		if (-1 == pid) {
			if (ECHILD != errno) {
				perror ("waitpid");
				exit (-1);
			}
		
		printf ("服务器：全部子进程都已退出。\n");
		break;
		}
		
		if (pid) 
			printf ("服务器：发现%u子进程退出。\n", pid);
		else {
			printf ("服务器：暂时没有子进程退出。\n");
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if (2 > argc){
		fprintf (stderr, "Usage: %s < port>\n", argv[0]);
		return -1;
	}
	
	if (SIG_ERR == signal (SIGCHLD, sigchld)) {
		perror ("signal");
		return -1;
	}
	
	printf("服务器：创建网络数据报套接字...\n");
	int sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror ("socket");
		return -1;
	}

	printf("服务器：准备地址并绑定...\n");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (atoi (argv[1]));
	//addr.sin_port = htonl (atoi (argv[1]));
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (-1 == bind (sockfd, (struct sockaddr*)&addr, sizeof (addr))) {
		perror ("bind");
		return -1;
	}

	printf("服务器：监听套接字...\n");
	if (-1 == listen (sockfd, 1024)){
		perror ("listen");
		return -1;
	}

	while(1) {
		printf ("服务器：等待连接请求......\n");
		
		struct sockaddr_in addrcli = {};
		socklen_t addrlen = sizeof (addrcli);
		int connfd = accept (sockfd, (struct sockaddr*)&addrcli, &addrlen);
		if (-1 == connfd){
			perror ("accept");
			return -1;
		}
	
		printf ("服务器：接受来自%s:%u客服机的连接请求。创建子进程为其提供服务...\n",
				inet_ntoa (addrcli.sin_addr), ntohs (addrcli.sin_port));
			
		pid_t pid = fork ();
		if (-1 == pid) {
			perror ("fork");
			return -1;
		}
		if (0 ==  pid) {
			printf ("%u子进程： 为%s:%u客户机提供服务...\n", getpid(), 
					inet_ntoa (addrcli.sin_addr), ntohs (addrcli.sin_port));
			if (-1 == close (sockfd)) {
				perror ("close");
				return -1;
			}
			while (1){
				char buf[1024];
				printf ("%u子进程：接收请求...\n",getpid());
				ssize_t rb = recv (connfd, buf, sizeof(buf), 0);				
				if (-1 == rb) {
					perror ("recv");
					return -1;
				}
				if (0 == rb) {
					printf ("%u子进程：客户机已关闭连接...\n", getpid());
					break;
				}
				
				printf ("%u子进程：发送响应...\n", getpid());
				
				if (-1 == send (connfd, buf, rb, 0)) {
					perror ("send");
					return -1;
				}
			}
			
			printf ("%u子进程：关闭连接套接字...\n", getpid());
			
			if (-1 == close (connfd)) {
				perror ("close");
				return -1;
			}
			
			printf ("%u子进程：即将退出。\n", getpid ());
			
			return 0;
		}
		
		if (-1 == close (connfd)) {
			perror ("close");
			return -1;
		}
		
	}

	printf("服务器：关闭监听套接字...\n");
	if (-1 == close (sockfd)) {
		perror ("close");
		return -1;
	}

	printf("服务器：一切OK...\n");

	return 0;
}
