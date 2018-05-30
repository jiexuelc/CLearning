#include <stdio.h>

int main()
{
    int i,num,a;
	unsigned long long int value=0,sum;
	printf("Please input value of n and a\n");
	scanf("%d%d",&num,&a);
	sum=value=a;
	printf("%d",a);
	for(i=1;i<num;i++)
	{
		value=value*10+a;
		sum=sum+value;
		printf("+%llu",value);
	}
	printf("=%llu\n",sum);
    return 0;
}