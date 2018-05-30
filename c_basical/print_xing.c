#include<stdio.h>
void main( )
{
int i,j,k;
for(i=1;i<=4;i++)              
    {
		for(j=1;j<=4-i;j++)     
		printf(" ");
		for(k=1;k<=i;k++)       
		printf("*");
		for(k=2;k<=i;k++)       
		printf("*");
		for(j=1;j<=4-i;j++)     
		printf(" ");
		printf("\n");  
	}
for(i=1;i<=3;i++)
	{
		for(j=1;j<=i;j++)     
		printf(" ");
		for(k=1;k<=4-i;k++)       
		printf("*");
		for(k=2;k<=4-i;k++)       
		printf("*");
		for(j=1;j<=i;j++)     
		printf(" ");
		printf("\n");  
	}
}
