#include<stdio.h>
#define N 12
int shuzu[3][4];
void main()
{
	void shuru(int shuzu[3][4]);
	void shuchu(int shuzu[3][4]);
	void sumavr(int shuzu[3][4]);
	void zhuanzhi(int shuzu[3][4],int shuzu1[4][3]);
	void maxmin(int shuzu[3][4]);
	void paixu(int shuzu2[10]);


	int a,shuzu1[4][3],shuzu2[10]; 
	   printf("             **********�������ϵͳ***************\n");
	   printf("                      1. ��������\n");
	   printf("                      2. �������\n");
	   printf("                      3. ������ͼ���ƽ��ֵ\n");
	   printf("                      4. ����ת��\n");
	   printf("                      5. �������Сֵ\n");
	   printf("                      6. �������򣨴��С��\n");
	   printf("                      7. �˳�ϵͳ\n");
	   printf("            **********��������ѡ����***********\n");
hui: printf("��ѡ��Ҫ����������ţ�");
	scanf("%d",&a);
	switch(a)
	{
		case 1: shuru(shuzu); break;
		case 2: shuchu(shuzu); break;
		case 3: sumavr(shuzu); break;
		case 4: zhuanzhi(shuzu,shuzu1) ; break;
		case 5: maxmin(shuzu); break;
		case 6: paixu(shuzu2) ; break;
		case 7:goto end ; break;
		default: printf("������Ϣ�������������룡");
	 }
	goto hui;
end:printf("�ɹ��˳�ϵͳ��\n");
}

 void shuru(int shuzu[3][4])
{
	int i,j;
	printf("������һ��3*4�����飺");
	for(i=0;i<3;i++){
		for(j=0;j<4;j++)
			scanf("%d",&shuzu[i][j]);
	}
}

void shuchu(int shuzu[3][4])
{
	int *p;
	printf("������飺\n");
	for(p=shuzu[0];p<shuzu[0]+12;p++)
	{
		if((p-shuzu[0])%4==0)   printf("\n");
		printf("%5d",*p);
	}
	printf("\n");
}

void sumavr(int shuzu[3][4])
{
	int i,j;
	float sum=0;
	float avr=0;
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		sum=sum+shuzu[i][j];
	}
	avr=sum/N;
	printf("sum= %5.0f  ave= %f\n",sum,avr);
}

void zhuanzhi(int shuzu[3][4],int shuzu1[4][3])
{
	int i,j,t;
	for(i=0;i<3;i++)
	{
		for(j=0; j<4; j++) 
		shuzu1[j][i]=shuzu[i][j];
	}
	for(i=0;i<4;i++){
		for(j=0;j<3;j++)
			printf("%5d",shuzu1[i][j]);	
	printf("\n");
	}
}

void maxmin(int shuzu[3][4])
{
	int i,j,max,min,m;
	max=shuzu[0][0];
	min=shuzu[0][0];
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++){
			if(max<shuzu[i][j]){
				max=shuzu[i][j];
				}
			if(min>shuzu[i][j]){
				min=shuzu[i][j];
			}
		}
	}
	printf("max=%d\n",max);
	printf("min=%d\n",min);
}

void paixu(int*shuzu2)
{
	int i,j, t,*p;
	printf("������ʮ��������\n");
	for(i=0;i<10;i++)
	scanf("%d",(shuzu2+i));
	printf("�ɴ�С��˳��Ϊ��");
	p=&shuzu2;
	for(i=0;i<9;i++){
		for(j=i+1;j<10;j++)
			if(*(shuzu2+j)>*(shuzu2+i)){
				t=*(shuzu2+j);*(shuzu2+j)=*(shuzu2+i);*(shuzu2+i)=t;
				}
	}
	for(p=shuzu2,i=0;i<10;i++){
		printf("%5d",*p);p++;
		}
	printf("\n");
}

