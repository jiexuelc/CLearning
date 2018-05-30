#include "time.h"
#include <stdio.h>
#include <stdlib.h>
int main () 
{ 
int flag;
printf("***************************************\n");
printf("***********获取当前系统时间************\n");
printf("***************************************\n\n");
printf("获取请输入：1\n");
printf("结束请输入：0\n");
scanf("%d",&flag);
while(flag)
{
	time_t rawtime; 
	struct tm * timeinfo; 
	time ( &rawtime ); 
	timeinfo = localtime ( &rawtime ); 
	printf ( "The current date/time is: %s", asctime (timeinfo) ); 
	printf("本次输出结束\n\n");
	printf("再次获取请输入：1\n");
	printf("结束程序请输入：0\n");
	scanf("%d",&flag);
}
exit;
return 0;
} 
