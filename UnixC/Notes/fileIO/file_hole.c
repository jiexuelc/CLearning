/*************************************************************************
	> File Name: file_hole.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月21日 星期日 15时34分45秒
	> Description: 创建一个具有文件空洞的文件，文件空洞不要求占用硬盘块 
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;

    if(0 > (fd = creat("file.hole", 0666)))
    {
        printf("creat error");
    }

    if(10 != write(fd, buf1, 10))
    {
        printf("buf1 write error");
    }
    /* offset now = 10 */

    if(-1 == lseek(fd, 16384, SEEK_CUR))
    {
        printf("lseek error");
    }
    /* offset now = 10 + 16384 */

    if(10 != write(fd, buf2, 10))
    {
        printf("buf2 write error");
    }
    /* offset now = 16404 */

    //close(fd);

    return 0;
}
