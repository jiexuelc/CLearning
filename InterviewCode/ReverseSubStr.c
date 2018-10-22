/*************************************************************************
	> File Name: ReverseSubStr.c
    > Author: jiexue
    > Mail: jiexuelc@163.com
	> Created Time: Sat 20 Oct 2018 11:17:59 PM CST
    > Description: 将字符串中包含的子串倒序输出
                例如：输入"do you do how"，输出"how do you do"
 ************************************************************************/

#include <stdio.h>

void ReverseSubStr(char* str) {
    char* head = str; //保存字符串头地址
    char* temp; //用于存储子串开始地址的临时变量

    if (str == NULL) {
        return;
    }

    while(*++str != '\0');    //定位到字符串末尾
    //str--;

    while(str != head) {
        for(; (*str != ' ') && (str != head); --str);  //从字符串末尾反向找空格或者字符串头，以此定位单词起始地址

        /* 如果当前为空格，说明空格后面为单词首地址 */
        if(*str == ' ') {
            temp = str + 1;
        } else { //当前为子串首地址，则当前为单词的起始地址
            temp = head;
        }

        /* 输出单词 */
        for(; *temp != ' ' && *temp != '\0'; temp++) {
            printf("%c", *temp);
        }

        /* 如果已经是最后一个单词，不需要输出空格。其余情况需要跳过原字符串的空格，以便进行下一个单词定位 */
        if(str != head) {
            printf(" ");
            --str;
        }
    }
    printf("\n");
}

int main()
{
    char a[] = "do you do how";
    char b[] = "you? And fine. I'm";
    char c[] = "me like someone honoring this like night a of part be to lucky be would Anybody";
    ReverseSubStr(a);
    ReverseSubStr(b);
    ReverseSubStr(c);
    
    return 0;
}
