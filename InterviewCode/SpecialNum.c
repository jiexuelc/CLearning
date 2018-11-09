/*************************************************************************
	> File Name: SpecialNum.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Sat 13 Oct 2018 08:07:28 PM CST
	> Description: 输出特殊序列第n个数是多少
                例如：11135917171513...
                      12361181091010...
                前三位数字为初始输入值，从第4位数开始等于前三位数相加，相加大于10则分为两位进行存储
 ************************************************************************/

#include<stdio.h>

int main()
{
    char a1[3] = {}; //用于存放初始的三位数
    char a[3] = {};  //该数组变量用于循环存放处理序列
    printf("请输入初始的三位数,每位以空格隔开：\n");
    scanf("%c %c %c", &a1[0], &a1[1], &a1[2]);
    int pos;    //序列位置索引
    int sum;    //存放三位数相加的和
    int n;      //存放想得到的第n个位置
    char shi, ge;   //将计算得到的数字转换成字符数字存放(因三位数相加不可能超过2位数，故两位足够)

    for(;;){
        printf("输入想得到的第几位数字：\n");
        scanf("%d", &n);
        pos = 2;    //由于初始数组里面已经有三位数，所以这里pos为2
        n = n - 1;  //减1,为了和数组位置索引统一起来
        sum = 0;
        /* 循环数组初始化 */
        a[0] = a1[0];   
        a[1] = a1[1];
        a[2] = a1[2];
        
        while(pos < n){ //循环结束条件pos == n 或者pos == n+1(当所要求得的位置前三位加起来大于10会出现此种情况)
            sum = a[0] + a[1] + a[2] - 3 * '0';
            
            /* sum大于等于10则分为两位存放 */
            if(sum >= 10){
                shi = sum/10 + '0';
                ge = sum%10 + '0';
                
                /* 循环存放在数组中，节约内存开销 */
                a[(++pos)%3] = shi;
                a[(++pos)%3] = ge;
            } else {    //sum不大于10时，只需要一位就可以存放
                a[(++pos)%3] = sum + '0';
            }
        }

    printf("%c\n", a[n%3]); //输出对应数组索引的数即为要求的数(注意这里的n不能换成pos)
    }

    return 0;
}