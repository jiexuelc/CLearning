#include<stdio.h>
void main()
{
int array[3][4],trans_array[4][3];
int i,j;
printf("Please input 3*4 array:\n");
	for(i=0;i<3;i++)
		for(j=0;j<4;j++)
		{
			scanf("%d",&array[i][j]);
			trans_array[j][i]=array[i][j];
		}
printf("array[3][4]:\n");
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		{
		printf("%6d ",array[i][j]);
		}
		printf("\n");
	}
printf("trans_array[4][3]:\n");
	for(i=0;i<4;i++)
	{
		for(j=0;j<3;j++)
		{
		printf("%6d ",trans_array[i][j]);
		}
		printf("\n");
	}
}