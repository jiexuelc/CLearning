#include<stdio.h>
#include<stdlib.h>
int main(void)
{
	int flag;
	printf("***********��������������ȶ�����ʮ����¶�ֵ************\n");
	printf("***********����ֵΪ1/KT��ֵ������¶�Ϊ���϶�************\n\n");
	printf("���������룺1\n");
	printf("�˳������룺0\n");
	scanf("%d",&flag);
	while(flag)
	{
		float Tout,Bin,Kb;
		Kb=0.00008617;
		printf("������Bin��ֵ:\n");
		scanf("%f",&Bin);	
		Tout=1/(Bin*Kb)-273.15;
		printf("ʮ�����ݱ��ֵ��¶�Ϊ��%.4f\n",Tout);
		printf("���μ������\n\n");
		printf("���������룺1\n");
		printf("�˳����������룺0\n");
		scanf("%d",&flag);
	}
	exit;
	return 0;
}