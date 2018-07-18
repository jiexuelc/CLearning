/*************************************************************************
    > File Name: IPC_fifo.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月18日 星期三 19时49分51秒
    > Description: 有名管道写端
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define FIFO_FILE "fifo"

int main() 
{
	printf ("创建管道...\n");

	if (-1 == mkfifo (FIFO_FILE, 0666)){
		perror ("mkfifo");
		return -1;
	}

	printf ("打开管道...\n");
	int fd = open (FIFO_FILE,O_WRONLY);
	if (-1 == fd) {
		perror ("open");
		return -1;
	}

	printf ("发送数据...\n");

	while (1){
		printf ("> ");

		char buf[1024];
		//gets (buf);
		scanf ("%s",buf);
		if (!strcmp (buf, "!"))
			break;
		if (-1 == write (fd, buf, (strlen (buf)+1) * sizeof (buf[0]))) {
			perror ("write");
			return -1;
		}
	}

	printf ("关闭管道...\n");
	if (-1 == close (fd)){
		perror ("close");
		return -1;
	}

	printf ("删除管道...\n");
	if (-1 == unlink (FIFO_FILE)) {
		perror ("unlink");
		return -1;
	}

	printf ("任务完成...\n");
	return 0;	
}
