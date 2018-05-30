#include<stdio.h>

long int power(int n)
{
long int c;
if (n<0)
	printf("n<0,error\n");
else if (n==1||n==0)
		c=1;
else
	c=power(n-1)*n;
return c;
}

void main()
{
	int n;
	long int value;
	printf("input an integer number:\n");
	scanf("%d",&n);
	value=power(n);
	printf("%ld\n",value);
}