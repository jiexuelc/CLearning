#include<stdio.h>
void main()
{
int array[3][4];
int i,j,max,row,column;
printf("Please input 3*4 array:\n");
	for(i=0;i<3;i++)
		for(j=0;j<4;j++)
			scanf("%d",&array[i][j]);
	max=array[0][0];
	for(i=0;i<3;i++)
		for(j=1;j<4;j++)
			{
			if(max<array[i][j])
				{
				max=array[i][j];
				row=i+1;
				column=j+1;
				}
			}
printf("max=%d\n",max);
printf("row=%d  column=%d\n",row,column);
}