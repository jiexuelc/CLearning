/*************************************************************************
	> File Name: delete_char.c
	> Author: jiexue 
	> Mail: jiexuelc@163.com
	> Created Time: Thu 18 Oct 2018 10:12:06 PM CST
    > Description: 从str1中删除str2中包含的字符
                例如：char str1[] = "Abc*defghicc";
                      char str2[] = "Ac*ic";
                  结果为：bdefghc
 ************************************************************************/

#include<stdio.h>

void delete_char(char* str1, char* str2)
{
    char *head = str1;  //记住子符串1首地址
    char *back;   //临时指针变量

    for(; *str2 != '\0'; str2++) {    //依次遍历str2中的每一个字符
        /* 从左往右在字符串str1中匹配第一个与str2中相同的字符，并将后面的字符依次往前挪动一位 */
        for(str1 = head; *str1 != '\0'; str1++) {
            if(*str2 == *str1) {
                back = str1 + 1;
                while(*str1) {
                    *str1++ = *back++;
                }
                //*str1 = '\0';
            }
        }
    }
}

int main()
{
    char str1[] = "Abc*defghicc";
    char str2[] = "Ac*ic";
    delete_char(str1, str2);

    printf("%s\n", str1);
    return 0;
}
