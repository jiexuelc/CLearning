#include<stdio.h>
void swap(int *sw1,int *sw2)
{
	int temp;
	temp=*sw1;
	*sw1=*sw2;
	*sw2=temp;
}
void sort(int *so1,int *so2,int *so3)
{
if(*so1<*so2) swap(so1,so2);
if(*so1<*so3) swap(so1,so3);
if(*so2<*so3) swap(so2,so3);
}
void main()
{
	int a,b,c;
	int *pa,*pb,*pc;
	scanf("%d %d %d",&a,&b,&c);
	pa=&a;pb=&b;pc=&c;
	sort(pa,pb,pc);
	printf("%d %d %d\n",a,b,c);
}