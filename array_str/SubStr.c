/*************************************************************************
	> File Name: SubStr.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月13日 星期五 14时22分17秒
	> Description: 把字符串用指定的字符作为分隔符分割成若干个子串输出
 ************************************************************************/

#include<stdio.h>

void substr(char* str, char DivCh)
{
     /*for(;*str != '\0';){
         if(*str == DivCh){
             printf("\n");
             str++;
         }
         else{
             printf("%c",*str);
             str++;
         }
     }*/
     do{
        if(*str == DivCh)
            printf("\n");
        else printf("%c",*str);
    }while(*++str != '\0');
    printf("\n");
}



int main()
{
    char str[100];
    char DivCh;
    printf("请分别输入一串字符串与子字符串分隔符\n");
    scanf("%s %c",str,&DivCh);   
    /*scanf("%s",str)
    scanf("%*[^\n");
    scanf("%*c");
    scanf("%c",&DivCh);*/
    substr(str,DivCh);
    return 0;
}
