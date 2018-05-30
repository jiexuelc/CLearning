#include <stdio.h>

int Fibonacci(int n)
{
	if (n==0 | n==1){
		return 1;
	}else {
		int result=Fibonacci(n-1)+Fibonacci(n-2);
		return result;
	}	
}

int main(void)
{
	int value,n;
	printf("Please input n:\n");
	scanf("%d",&n);
	value=Fibonacci(n);
	printf("%d\n",value);
	return 0;
}