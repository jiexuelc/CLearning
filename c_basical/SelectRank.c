#include<stdio.h>
void main()
{
	long int array[10]={0};
	int i,j,t;
	printf("Please input 10 numbers:\n");
	for(i=0;i<10;i++)
	scanf("%ld",&array[i]);
	for(i=0;i<9;i++)
		for(j=i+1;j<10;j++)
			if(array[i]>array[j])
			{
			t=array[i];
			array[i]=array[j];
			array[j]=t;
			}
	printf("The sorted numbers:\n");
	for(i=0;i<10;i++)
		printf("%ld  ",array[i]);
	printf("\n");
}