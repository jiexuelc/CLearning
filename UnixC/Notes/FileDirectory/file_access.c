/*************************************************************************
	> File Name: file_access.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月23日 星期二 20时14分18秒
	> Description: 获取输入文件权限 
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(2 != argc)
    {
        printf("输入参数有误！\n");
        printf("Usage: file_access <filepath>\n");
        return 0;
    }

    if(-1 == access(argv[1], F_OK))
    {
        printf("error\n");
        return 0;
    } else {
        printf("该文件具有：");
        if(-1 == access(argv[1], R_OK))
        {
            printf("read test error\n");
            return 0;
        } else {
            printf("读");
        }

        if(-1 == access(argv[1], W_OK))
        {
            printf("write test error\n");
            return 0;
        } else {
            printf("写");
        }

        if(-1 == access(argv[1], X_OK))
        {
            printf("exc test error\n");
            return 0;
        } else {
            printf("执行");
        }

        printf("权限\n");
    }

    return 0;
}
