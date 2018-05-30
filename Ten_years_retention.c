#include<stdio.h>
#include<stdlib.h>
int main(void)
{
	int flag;
	printf("***********此软件计算数据稳定保存十年的温度值************\n");
	printf("***********输入值为1/KT的值，输出温度为摄氏度************\n\n");
	printf("计算请输入：1\n");
	printf("退出请输入：0\n");
	scanf("%d",&flag);
	while(flag)
	{
		float Tout,Bin,Kb;
		Kb=0.00008617;
		printf("请输入Bin的值:\n");
		scanf("%f",&Bin);	
		Tout=1/(Bin*Kb)-273.15;
		printf("十年数据保持的温度为：%.4f\n",Tout);
		printf("本次计算结束\n\n");
		printf("计算请输入：1\n");
		printf("退出程序请输入：0\n");
		scanf("%d",&flag);
	}
	exit;
	return 0;
}