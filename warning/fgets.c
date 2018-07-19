/*************************************************************************
    > File Name: fgets.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月19日 星期四 8时42分30秒
    > Description: fgets用法：
				   1.当输入字符数 <= size-2，将在字符串末尾自动添加'\n' '\0'两个字符
				   2.当输入字符数 >= size-1，将在字符串末尾自动添加'\0'一个字符
				   3.成功返回字符串buf地址，失败或到读到文件尾返回NULL
				   查看文件真实内容：od -A d -c <file>
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int fd = open ("file", O_RDWR | O_CREAT | O_TRUNC);
	if (-1 == fd) {
		return -1;
	}
	
	char buf[10];
	buf[9] = 'a';
	char* p = fgets (buf, sizeof (buf), stdin);
	printf ("%d\n", strlen (buf));
	
	int rb = write (fd, buf, sizeof (buf));
	if (-1 == rb) {
		perror ("write");
		return -1;
	}
	close (fd);
	return 0;
}
