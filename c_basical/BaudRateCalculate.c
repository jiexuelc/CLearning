#include<stdio.h>
void main()
{
	while(1)
	{
		long int f,bps;
		int SMOD,smodv,TLv,TL;
		printf("请输入晶振频率f:\n");
		scanf("%ld",&f);
		printf("请输入SMOD的高低电平0或1:\n");
		scanf("%d",&SMOD);
		printf("请输入波特率bps:\n");
		scanf("%ld",&bps);
		if(SMOD==0)
			smodv=1;
		else smodv=2;
		TL=(int)((f*smodv)/(32*bps*12)+0.5);
		if(TL>=1)
		{
			TLv=256-TL;
			printf("%x \n",TLv);
			printf("\n");
		}
		else 
			printf("你输入有误，请重新输入！\n");
	}
}