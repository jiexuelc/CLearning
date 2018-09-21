/*************************************************************************
	> File Name: stdarg.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Thu 20 Sep 2018 22:35:14 CST
	> Description: 变长参数表
 ************************************************************************/

#include <stdio.h>
#include <stdarg.h>

int maxValue(int num, ...)  //...代表不定长参数，其中第一个参数"int num"不可省略
{
    int max, temp;
    int i;
    va_list v;      //新建变长参数表
    va_start(v, num);   //用V保存cnt之后的所有参数
    max = va_arg(v, int);   //从参数表中取出一个int型参数,取出一个参数之后，自动指向下一个参数，不可直接跳跃访问

    for(i = num - 1; i > 0; --i)
    {
        temp = va_arg(v, int);  //从参数表中取出下一个int型参数
        if(temp > max)
            max = temp;
    }
    va_end(v);  //释放可变长参数表v

    return max;
}

int main()
{
    int max, min;

    max = maxValue(5, 4, 8, 10, 45, 100);
    printf("max: %d\n", max);

    max = maxValue(6, 3, 4, 5, 6, 7, 8);
    printf("max: %d\n", max);

    return 0;
}
