#include<stdio.h>
void trans_code()
{
int i;
char c,code[50]={0};
for(i=0;(c=getchar())!='\n';i++)
	{
	if(c>='A'&&c<='Z'||c>='a'&&c<='z')
		{
		c=c+4;
		if(c>'Z'&&c<='Z'+4||c>'z')
			c=c-26;
		}
	code[i]=c;
	}
for(i=0;code[i]!='\0';i++)
	{
	printf("%c",code[i]);
	}
printf("\n");
}

void trans_text()
{
int i;
char c,code[50]={0};
for(i=0;(c=getchar())!='\n';i++)
	{
	if(c>='A'&&c<='Z'||c>='a'&&c<='z')
		{
		c=c-4;
		if(c>=61&&c<'A'||c>=93&&c<'a')
			c=c+26;
		}
	code[i]=c;
	}
for(i=0;code[i]!='\0';i++)
	{
	printf("%c",code[i]);
	}
printf("\n");
}

void main()
{
trans_code();
trans_text();
}