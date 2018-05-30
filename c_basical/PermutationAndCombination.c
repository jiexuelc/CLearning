#include<stdio.h>
void main()
{
	int red=0,white=0,black=0,num=0;
	for(red=0;red<=3;red++)
		for(white=0;white<=3;white++)
			for(black=0;black<=6;black++)
				if(red+white+black==8) num++;
				printf("一共有%d种可能\n",num);
}