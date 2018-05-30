#include <stdio.h>

void Product9x9(void)
{
	int i,j;
	for(i=1;i<=9;i++)
	{
		for(j=1;j<=i;j++)
			printf("%d*%d=%d\t",j,i,i*j);
		printf("\n");
	}
}

int main(void)
{
	Product9x9();
	return 0;
}