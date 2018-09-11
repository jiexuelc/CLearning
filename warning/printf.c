/*************************************************************************
	> File Name: printf.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Tue 11 Sep 2018 02:51:38 PM CST
	> Description: 知识点：
                    １．'++'运算符的优先级大于'*'
                    ２．'++'和'*'运算符的结合方向为从右向左
                    ３．printf()函数是从右向左计算表达式，输出时按从左往右输出
 ************************************************************************/

#include <stdio.h>

int main()
{
    char a[] = "hello, world";
    char *p = a;
    
    /*
     * p优先进行自加操作，然后从右向左与*结合进行解地址运算得到值
     */
    printf("%c %c\n", *p,*++p); //输出e e
    p = a;
    
    /*
     * 尽管++的优先级高，但是*解地址运算却取的是++之前的地址
     */
    printf("%c %c %c\n", *p,*p++,*p++); //输出l e h
    
    int i = 0;
    printf("%d %d %d %d\n", i, ++i, i++, i++); //输出3 3 1 0
    
    return 0;
}
