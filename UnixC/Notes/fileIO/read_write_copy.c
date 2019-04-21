/*************************************************************************
	> File Name: read_write_copy.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月21日 星期日 18时04分40秒
	> Description: 使用read和write函数完成copy操作
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int fd_s;
    int fd_t;
    ssize_t RetVal;
    char buf[4096];
    
    if(3 != argc)
    {
        printf("输入参数错误！\n");
        printf("Usage:read_write_copy <Source file> <Target file>\n");
        return 0;
    }
    
    if(-1 == (fd_s = open(argv[1], O_RDONLY))) //只读打开
    {
        printf("打开源文件出错！\n");
        return 0;
    }
    
    if(-1 == (fd_t = open(argv[2], O_RDWR | O_CREAT | O_EXCL, 0664)))
    {
        printf("目标文件创建失败！\n");
        return 0;
    }
    
    //循环读取，循环写入
    while(RetVal = read(fd_s, buf, sizeof(buf) / sizeof(char)))
    {
        if(-1 == RetVal)
        {
            printf("读源文件出错!\n");
            break;
        } else {
            if(RetVal != write(fd_t, buf, RetVal))
            {
                printf("数据写入目标文件出错！\n");
                break;
            }
        }
    }
    
    if(-1 == close(fd_s))
    {
        printf("关闭源文件失败！\n");
    }
    
    if(-1 == close(fd_t))
    {
        printf("关闭目标文件失败！\n");
    }
    
    return 0;
}
