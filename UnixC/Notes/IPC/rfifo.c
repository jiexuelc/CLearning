/*************************************************************************
    > File Name: IPC_fifo.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月18日 星期三 19时49分51秒
    > Description: 有名管道读端
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO_FILE "fifo"

int main() 
{
	printf ("打开管道...\n");
	int fd = open (FIFO_FILE,O_RDONLY);
	if (-1 == fd) {
		perror ("open");
		return -1;
	}

	printf ("接收数据...\n");

	while (1){
		char buf[1024];
		ssize_t rb =  read (fd, buf, sizeof (buf));
		if (-1 == rb) {
			perror ("read");
			return -1;
		}
		if (0 == rb){
			break;
		}
		printf ("< %s\n", buf);
	}

	printf ("关闭管道...\n");
	if (-1 == close (fd)){
		perror ("close");
		return -1;
	}
	printf ("任务完成...\n");
	return 0;	
}
