/*************************************************************************
	> File Name: getFileType.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月22日 星期一 19时45分08秒
	> Description: 获取文件类型
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    
    struct stat buf;
    const char* ptr;
    
    if(2 != argc)
    {
        printf("输入参数错误！\n");
        printf("Usage: getFileType <filename>\n");
        return 0;
    }
    
    if(-1 == lstat(argv[1], &buf))
    {
        printf("获取文件类型错误！\n");
        return 0;
    }
    
    if(S_ISREG(buf.st_mode))
    {
        ptr = "regular";
    }
    else if(S_ISDIR(buf.st_mode))
    {
        ptr = "directory";
    }
    else if(S_ISCHR(buf.st_mode))
    {
        ptr = "character special";
    }
    else if(S_ISBLK(buf.st_mode))
    {
        ptr = "block special";
    }
    else if(S_ISFIFO(buf.st_mode))
    {
        ptr = "fifo";
    }
    else if(S_ISLNK(buf.st_mode))
    {
        ptr = "symbolic link";
    }
    else if(S_ISSOCK(buf.st_mode))
    {
        ptr = "socket";
    }
    else
    {
        ptr = "ubknown mode";
    }
    
    printf("%s\n", ptr);
    
    return 0;
}
