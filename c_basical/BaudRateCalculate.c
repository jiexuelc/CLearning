#include<stdio.h>
void main()
{
	while(1)
	{
		long int f,bps;
		int SMOD,smodv,TLv,TL;
		printf("�����뾧��Ƶ��f:\n");
		scanf("%ld",&f);
		printf("������SMOD�ĸߵ͵�ƽ0��1:\n");
		scanf("%d",&SMOD);
		printf("�����벨����bps:\n");
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
			printf("�������������������룡\n");
	}
}