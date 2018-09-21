/*************************************************************************
	> File Name: ppMalloc.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Fri 21 Sep 2018 08:21:52 PM CST
	> Description: 二级指针用于内存申请典型应用举例
 ************************************************************************/

#include <stdio.h>
#include <string.h>

/*
 * 内存申请
 */
void setmemory(char** p, int num)
{
    *p = (char*) malloc (num * sizeof(char));
}

/*
 * 内存释放
 */
void FreeMemory(char** p)
{
    free(*p);
    *p = NULL;
}

int main()
{
    char* str = NULL;
    
    setmemory(&str, 100);
    strcpy(str,"hello pointer pointer\n");
    printf("Value of string: %s", str);
    FreeMemory(&str);
    
    return 0;
}
