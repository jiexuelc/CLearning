/*************************************************************************
	> File Name: days.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月09日 星期一 20时50分11秒
	> Description: 计算天数
 ************************************************************************/

#include"days.h"
#include"LeapYear.h"
#include"IsDay.h"

unsigned int days(unsigned int year,unsigned int month,unsigned int day)
{
    unsigned int i,sum=day;
    if(month>1){
        for(i=1;i<month;i++){
            if(i==1 || i==3 || i==5 || i==7 || i==8 || i==10 || i== 12){
                sum += 31;continue;
            }
            else if(i==2){
                if(LeapYear(year)){
                    sum +=29;
                }
                else sum +=28;
                continue;
            }
            else sum +=30;
        }
    }
    return sum;
}
