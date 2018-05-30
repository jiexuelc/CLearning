#include<stdio.h>
void search(float (*p)[4],int n)
{
	int i,j,flag;
	for(i=0;i<n;i++)
		{
		flag=0;
		for(j=0;j<4;j++)
			if(*(*(p+i)+j)<60) flag=1;
		if(flag==1)
			{
			printf("No.%d fails,his score are:\n",i+1);
			for(j=0;j<4;j++)
				printf("%5.1f",*(*(p+i)+j));
			printf("\n");
			}
		}
}

void main()
{
	int i,j;
	float score[3][4]={0};
	for(i=0;i<3;i++)
		for(j=0;j<4;j++)
			scanf("%f",&score[i][j]);
	search(score,3);
}