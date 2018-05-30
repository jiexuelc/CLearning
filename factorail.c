#include <stdio.h>

int factorial(int n)
{
	if(n==0){
		return 1;
	}
	if(n!=0)
	{
		int result=n*factorial(n-1);
		return result;
	}
}

int main(void)
{
	int value;
	value=factorial(3);
	printf("%d\n%d\n",value);
	return 0;
}