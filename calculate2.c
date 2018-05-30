#include <stdio.h>
#include <math.h>
int main()
{
	unsigned long long int value=0,sum=0;
	int i;
	printf("2");
	for(i=2;i<=64;i++)
	{
		value=pow(2,i);
		sum=sum+value;
		printf("+%llu",value);
	}
	printf("=%llu\n",sum);
    return 0;
}