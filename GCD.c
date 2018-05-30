#include <stdio.h>

int Greatest_Common_Divisor(int a,int b)
{
	if (a%b==0)	{
		return b;
	}else{
		int result=Greatest_Common_Divisor(b,a%b);
		return result;
	}	
}

int main(void)
{
	int value,numa,numb;
	printf("Please input two positive integers:\n");
	scanf("%d %d",&numa,&numb);
	value=Greatest_Common_Divisor(numa,numb);
	printf("%d\n",value);
	return 0;
}
