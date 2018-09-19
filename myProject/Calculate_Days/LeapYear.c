/*************************************************************************
	> File Name: LeapYear.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月09日 星期一 19时17分42秒
	> Description: 判断是否闰年
 ************************************************************************/

#include"LeapYear.h"

bool LeapYear(unsigned int year)
{
    return ((year%4==0&&year%100!=0) || year%400==0);
}
