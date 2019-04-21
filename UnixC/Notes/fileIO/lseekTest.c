/*************************************************************************
	> File Name: lseekTest.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2019年04月19日 星期五 19时33分48秒
	> Description:  测试文件是否可以设置偏移量
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    if(-1 == lseek(STDIN_FILENO, 0, SEEK_CUR))
    {
        printf("file can not seek!\n");
    } else {
        printf("seek OK!\n");
    }

    return 0;
}
