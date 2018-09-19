/*************************************************************************
	> File Name: conststr.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月08日 星期日 10时39分30秒
	> Description: 字符串操作
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main()
{
    char a[100] = "Hello world!";
    char b = a;     //将a地址低8位存在b里面
    printf("%p\n%p\n", a, b);
    printf("%d\n", sizeof("Hello world!"));
    puts(a);
    printf("%d\n", sizeof(a));  //13
    printf("%d\n", *a);     //72(H)
    printf("%c\n", "jiexue2018"[3]);    //x
    printf("%s\n", "jiexue2018"+4);     //ue2018
    char cn[3][50] = {"中国", "你好吗？", "很好！！"};
    printf("%d\n%d\n%d\n", strlen(cn[0]), strlen(cn[1]), strlen(cn[2]));    //6 12 12
    
    return 0;
}
