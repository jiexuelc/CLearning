/*************************************************************************
	> File Name: set_FileFlags.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月22日 星期一 09时16分06秒
	> Description: 对指定文件描述符设置文件状态标志
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/*
 * 说明：设置文件状态标志
 * 输入参数：fd文件描述符
 *           flags文件状态标志
 * 输出参数：0 成功
 *           1 失败
 */
int set_FileFlags(int fd, int flags)
{
    int val;
    if(-1 == (val = fcntl(fd, F_GETFL, 0)))
    {
        printf("读取文件标志信息错误！\n");
        return 1;
    }
    
    val |= flags;
    
    if(-1 == fcntl(fd, F_SETFL, 0))
    {
        printf("设置文件标志信息错误！\n");
        return 1;
    }
    
    return 0;
}

int main(void)
{
    int val;
    
    val = set_FileFlags(STDOUT_FILENO, O_SYNC);
    
    if(0 == val)
    {
        printf("设置文件标志成功！\n");
    } else {
        printf("设置文件标志失败！\n");
    }
    
    return 0;
}
