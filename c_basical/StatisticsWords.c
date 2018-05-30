#include<stdio.h>
void main()
{
	char string[100];
	int word,num,i;
	num=0;
	word=0;
	gets(string);
	for(i=0;string[i]!='\0';i++)
		if(string[i]==' ')
			word=0;
		else if (word==0)
			{
			word=1;
			num++;
			}
	printf("There are %d words.\n",num);
}