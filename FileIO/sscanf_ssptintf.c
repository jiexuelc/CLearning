/*************************************************************************
	> File Name: sscanf_ssptintf.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Tue 25 Sep 2018 02:40:24 PM CST
	> Description: sscanf()与sprintf()分别从字符串读取数据和将输出数据放到字符串
                        int sprintf(char *str, const char *format, ...);
                        int sscanf(const char *str, const char *format, ...);
 ************************************************************************/

#include <stdio.h>

int main()
{
    char buf[100];
    sprintf(buf,"hello, i'm %s, today is %d-%d-%d",
            "jiexue", 2018,9,25);
    puts(buf);
    
    int age;
    char name[10];
    double salary;
    const char* str="25 Tom 20000";
    sscanf(str,"%d%s%lf", &age, name, &salary);
    printf("我是%s,今年%d,月薪%g\n", name, age, salary);
    
    return 0;
}

