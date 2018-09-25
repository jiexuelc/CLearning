/*************************************************************************
	> File Name: ppMalloc.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Fri 21 Sep 2018 08:21:52 PM CST
	> Description: 二级指针
                    1.用于内存申请典型应用举例
                    2.返回值同时返回指针
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

/*
 * 函数返回转换值，二级指针形参返回字符串转换的截止地址同时返回指向的值
 */
int str2int(const char* str, const char** q)
{
    int r = 0;
    while(isdigit(*str)){
        r = r * 10 + (*str - '0');
        ++str;
    }
    *q = str;
    return r;
}

int main()
{
    char* pstr = NULL;
    
    setmemory(&pstr, 100);
    strcpy(pstr,"hello pointer pointer\n");
    printf("Value of string: %s", pstr);
    FreeMemory(&pstr);
    
    const char* p=NULL;
    int value = str2int("3926abxys", &p);
    printf("value=%d, p=%s\n", value, p);
    return 0;
    
    return 0;
}
