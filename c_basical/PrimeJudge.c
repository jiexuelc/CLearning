#include<stdio.h>
#include<math.h>

int prime_judge(int num)
{
int i,SquareRoot;
SquareRoot=sqrt(num);
for(i=2;i<=SquareRoot;i++)
if(num%i==0) break;
if(i>SquareRoot) return 1;
else return 0;
}

void main()
{
int num,judge_num;
scanf("%d",&num);
judge_num=prime_judge(num);
if(judge_num==0)
	printf("%d is not a prime number\n",num);
else 
	printf("%d is a prime number\n",num);
}