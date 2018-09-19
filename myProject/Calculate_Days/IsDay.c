/*************************************************************************
	> File Name: IsDay.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月09日 星期一 18时43分30秒
	> Description: 判断日期是否合法
 ************************************************************************/

#include"IsDay.h"
#include"LeapYear.h"

bool isdate(unsigned int year,unsigned int month,unsigned int day)
{
    if(year>0){
        if(month>=1 && month<=12){
            if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12){
                if(day>0 && day<32){
                    return 1;
                }
                else return 0;
            }
            else if(month==4 || month==6 || month==9 || month==11){
                if(day>0 && day<31){
                    return 1;
                }
                else return 0;
            }
            if(month==2){
                if(LeapYear(year)){
                    if(day>0 && day<30){
                        return 1;
                    }
                    else return 0;
                }
                else {
                    if(day>0 && day<29){
                    return 1;
                }
                    else return 0;
                }
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}

