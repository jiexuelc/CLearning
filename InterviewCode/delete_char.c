/*************************************************************************
	> File Name: delete_char.c
	> Author: jiexue 
	> Mail: jiexuelc@163.com
	> Created Time: Thu 18 Oct 2018 10:12:06 PM CST
    > Description: 从str1中删除str2中包含的字符
 ************************************************************************/

#include<stdio.h>

void delete_char(char* str1, char* str2)
{
    char *temp, *head = str1;
    char *back;

    for(; *str2 != '\0'; str2++) {
        for(str1 = head; *str1 != '\0'; str1++) {
            if(*str2 == *str1) {
                back = str1 + 1;
                while(*str1) {
                    *str1++ = *back++;
                }
                *str1 = '\0';
            }
        }
    }
}

int main()
{
    char str1[] = "Abc*defghi";
    char str2[] = "Ac*i";
    delete_char(str1, str2);

    printf("%s\n", str1);
    return 0;
}
