/*************************************************************************
	> File Name: get_FileFlags.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月21日 星期日 20时07分35秒
	> Description: 对指定文件描述符获取并输出文件标志 
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int val;

    if(2 != argc)
    {
        printf("Usage: get_FileFlags <descriptor>\n");
        return 0;
    }

    if(-1 == (val = fcntl(atoi(argv[1]), F_GETFL, 0)))
    {
        printf("descriptor error");
    }

    switch(val & O_ACCMODE) {
    case O_RDONLY:
        printf("read only");
        break;

    case O_WRONLY:
        printf("write only");
        break;

    case O_RDWR:
        printf("read write");
        break;

    default:
        printf("ubknown access mode");
    }

    if(val & O_APPEND)
    {
        printf(", append");
    }
    if(val & O_NONBLOCK)
    {
        printf(",nonblocking");
    }
    if(val & O_SYNC)
    {
        printf(", synchronous writes");
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if(val & O_FSYNC)
    {
        printf(", synchronous writes");
    }
#endif
    putchar('\n');

    return 0;
}
