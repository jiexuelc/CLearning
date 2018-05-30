#include <stdio.h>

void diamond(int n,char c)
{
	int i,j,k;
    for(j=(n-1)/2;j>0;j--)
	{
        for(k=j;k>0;k--)
                printf(" \t");
        for(i=(n-2*j);i>0;i--)
                printf("%c\t",c);
        printf("\n");
    }
    for(j=0;j<=(n-1)/2;j++)
	{
        for(k=j;k>0;k--)
                printf(" \t");
        for(i=(n-2*j);i>0;i--)
                printf("%c\t",c);
        printf("\n");

    }
}

int main(void)
{
	diamond(9,'*');
	return 0;
}