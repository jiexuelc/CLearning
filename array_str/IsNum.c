/*************************************************************************
	> File Name: IsNum.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月13日 星期五 10时41分44秒
	> Description: 判断一组字符串是否全是数字字符
 ************************************************************************/

#include<stdio.h>
#include<string.h>

typedef enum  bool{FALSE = 0,TRUE = 1}bool;

//bool isnum(char* str, int N)
bool isnum(char* str)
{
    bool flag = FALSE;
    do{
        if(*str >= '0' && *str <= '9'){
            flag = TRUE;
        }
        else{
            return FALSE;
        }
    }while(*++str != '\0');

    return flag;
}

int main()
{
    char str[100];
    printf("请输入一串字符串：\n");
    scanf("%s",str);
    printf("%d\n",isnum(str));
   // printf("%d\n",isnum(str,strlen(str)));
    
    return 0;
}

