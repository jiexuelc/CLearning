/*************************************************************************
	> File Name: SpecialNum.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Sat 13 Oct 2018 08:07:28 PM CST
	> Description: 输出特殊序列第n个数是多少
                例如：11135917171513...
                      12361181091010...
                前三位数字为初始输入值，从第4位数开始等于前三位数相加，相加大于10分为两位进行存储
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char a1[3] = {};
    char a[3] = {};
    printf("请输入初始的三位数,每位以空格隔开：\n");
    scanf("%c %c %c", &a1[0], &a1[1], &a1[2]);
    int pos;
    int sum;
    int n;
    char shi, ge;

    for(;;){
        printf("输入想得到的第几位数字：\n");
        scanf("%d", &n);
        pos = 2;
        n = n - 1;
        sum = 0;
        a[0] = a1[0];
        a[1] = a1[1];
        a[2] = a1[2];
        
        while(pos < n){
            sum = a[0] + a[1] + a[2] - 3 * '0';
            if(sum >= 10){
                shi = sum/10 + '0';
                ge = sum%10 + '0';
                a[(++pos)%3] = shi;
                a[(++pos)%3] = ge;
            } else {
                a[(++pos)%3] = sum + '0';
            }
        }

    printf("%c\n", a[n%3]);
    }

    return 0;
}
