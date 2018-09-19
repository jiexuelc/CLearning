/*************************************************************************
	> File Name: Date.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月09日 星期一 19时37分47秒
	> Description: 计算输入的日期是该年的第多少天
 ************************************************************************/

#include<stdio.h>
#include"LeapYear.h"
#include"IsDay.h"
#include"days.h"

int main()
{
    unsigned int year,month,day;
    do{
        printf("请输入年(year)月(month)日(day)(以空格隔开):\n");
        scanf("%d %d %d",&year,&month,&day);
    }while(!isdate(year,month,day));
    printf("输入日期是%d年的第%d天\n", year, days(year,month,day));
    return 0;
}
