#include "time.h"
#include <stdio.h>
#include <stdlib.h>
int main () 
{ 
int flag;
printf("***************************************\n");
printf("***********��ȡ��ǰϵͳʱ��************\n");
printf("***************************************\n\n");
printf("��ȡ�����룺1\n");
printf("���������룺0\n");
scanf("%d",&flag);
while(flag)
{
	time_t rawtime; 
	struct tm * timeinfo; 
	time ( &rawtime ); 
	timeinfo = localtime ( &rawtime ); 
	printf ( "The current date/time is: %s", asctime (timeinfo) ); 
	printf("�����������\n\n");
	printf("�ٴλ�ȡ�����룺1\n");
	printf("�������������룺0\n");
	scanf("%d",&flag);
}
exit;
return 0;
} 
