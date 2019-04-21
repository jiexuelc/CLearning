/*************************************************************************
	> File Name: read_output.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月21日 星期日 16时33分30秒
	> Description: 将文件内容读取后输出到屏幕上
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int fd;
    ssize_t RetVal;
    char buf[10];
    char str[11];
    
    if(2 != argc)
    {
        printf("输入参数错误！\n");
        printf("Usage:read_output <filename>\n");
    }
    
    if(-1 == (fd = open(argv[1], O_RDONLY)))
    {
        printf("打开文件出错！\n");
    }
    
    while(RetVal = read(fd, buf, sizeof(buf) / sizeof(char)))
    {
        if(0 == RetVal)
        {
            return 0;
        }
        else if(-1 == RetVal) 
        {
            printf("读取出错!\n");
        } else {
            snprintf(str, RetVal + 1, "%s", buf);
            printf("%s", str);
        }
    }
    
    return 0;
}
