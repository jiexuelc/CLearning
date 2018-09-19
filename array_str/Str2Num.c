/*************************************************************************
    > File Name: Str2Num.c
    > Author: jiexue
    > Mail: jiexuelc@163.com
    > Created Time: Wed 19 Sep 2018 09:26:58 PM CST
    > Description: 将输入的字符串数字转换位整形数据存储
 ************************************************************************/

#include <stdio.h>
int main(void)
{
    int sum = 0, i = 0;
    char input[5];
    
    while (1) {
        scanf("%s", input);
        for (i = 0/*,sum=0*/; input[i] != '\0'; i++)
        sum = sum*10 + input[i] - '0';
        printf("input=%d\n", sum);
    }
    
    return 0;
}
