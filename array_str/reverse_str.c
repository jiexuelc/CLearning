/*************************************************************************
	> File Name: reverse_str.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Wed 26 Sep 2018 09:19:35 PM CST
	> Description: 将字符串反转进行输出
 ************************************************************************/

#include <stdio.h>
#include <string.h>

/*
 * 将字符串反转
 */
void reverse(char* p)
{
    
    int len = strlen(p);
    int half = len / 2;
    int i;
    char temp;
    
    for(i = 0; i < half; i++){
        temp = *(p+i);
        *(p+i) = *(p+len-1-i);
        *(p+len-1-i) = temp;
    }
}

int main()
{
    char a[] = "hello world";
    char b[] = "helloworld";
    reverse(a);
    reverse(b);
    printf("%s\n", a);
    printf("%s\n", b);
    
    return 0;
}
