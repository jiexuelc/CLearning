#include<stdio.h>
void main()
{
	int array[10]={0};
	int i,j,t;
	printf("Please input 10 numbers:\n");
	for(i=0;i<10;i++)
	scanf("%d",&array[i]);
	for(i=0;i<9;i++)
		for(j=0;j<9-i;j++)
			if(array[j]>array[j+1])
			{
			t=array[j];
			array[j]=array[j+1];
			array[j+1]=t;
			}
	printf("The sorted numbers:\n");
	for(i=0;i<10;i++)
		printf("%d  ",array[i]);
	printf("\n");
}