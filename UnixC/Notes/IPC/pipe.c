/*************************************************************************
    > File Name: pipe.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月18日 星期三 20时42分30秒
    > Description: 无名管道：父子进程通信
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
 * gets函数不检查越界(使用很危险)，fgets函数当字符串 <= size-2长度时包含换行符
 * 本函数利用标准输入与fgets函数封装成gets_s函数，实现gets函数的同时，可以避免越界
 */
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


int main() {
	printf ("父进程，创建管道...\n");

	int pipefd[2];
	if (-1 == pipe (pipefd)) {
		perror ("pipe");
		return -1;
	}
	
	printf ("父进程，创建进程...\n");

	pid_t pid = fork();
	if (-1 == pid) {
		perror ("fork");
		return -1;
	}
	if (0 == pid) {
		printf("我是子进程：关闭写端...\n");
		close (pipefd[1]);
		printf ("子进程，接收数据...\n");
		while (1) {
			char buf[1024];
			ssize_t rb = read (pipefd[0], buf, sizeof (buf));
			if (-1 == rb) {
				perror ("read");
				return -1;
			}

			if (0 == rb) {
				break;
			}
			printf("> %s\n", buf);
		}

		printf ("子进程，关闭读端...\n");
		close (pipefd[0]);
		printf ("子进程：成功关闭读端...\n");
		return 0;
	}
	
	printf ("父进程，关闭读端...\n");
	close (pipefd[0]);
	printf ("父进程，发送数据...\n");
	
	while (1) {
		char buf[1024];
		gets_s (buf, sizeof (buf));
		//scanf ("%s", buf);
		if (0 == strcmp (buf, "!"))
			break;
		if (write(pipefd[1], buf, (strlen(buf)+1) * sizeof(buf[0])) == -1) {
			perror ("write");
			return -1;
		}
	}

	printf ("父进程，关闭写端...\n");
	close (pipefd[1]);
	if (wait (0) == -1) {
		perror ("wait");
		return -1;
	}
	printf ("父进程，成功...\n");
	return 0;
}
