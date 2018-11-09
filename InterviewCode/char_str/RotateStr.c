/*************************************************************************
	> File Name: RotateStr.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Fri 09 Nov 2018 10:52:11 AM CST
	> Description: 给定一个字符串和一个偏移量，根据偏移量旋转字符串
                1.循环左移
                2.循环右移
                3.空间复杂度O(1)
 ************************************************************************/

#include <stdio.h>
#include <string.h>

/*
 * 循环左移函数
 * 算法思路：
 *   1.先将左边 offset 个字符串进行翻转；
 *   2.再将右边剩余字符串进行翻转;
 *   3.最后将整个字符串进行翻转.
 * 代码以字符串“abcXYZ789”循环左移3位进行注释
 */
void RotateLeft(char* str, int offset) {
    int len = strlen(str);
    char temp;      //用于交换
    int i, half;    //用于循环控制

    /* 对超过字符串长度的offset进行取余处理 */
    offset = offset % len;

    /* 刚好移动字符串长度的整数倍不需要处理 */
    if(0 == offset) {
        return;
    }

    /* 先将左边3个字符串进行翻转：abcXYZ789 --> cbaXYZ789 */
    half = offset / 2;
    for(i = 0; i < half; i++) {
        temp = str[i];
        str[i] = str[offset - i - 1];
        str[offset - i - 1] = temp; 
    }
    //printf("%s\n", str);

    /* 再将右边剩余字符串进行翻转：cbaXYZ789 --> cba987ZYX */
    half = (len - offset) / 2;
    for(i = 0; i < half; i++) {
        temp = str[offset + i];
        str[offset + i] = str[len - i - 1];
        str[len - i - 1] = temp; 
    }
    //printf("%s\n", str);

    /* 最后将整个字符串进行翻转： cba987ZYX --> XYZ789abc */
    half = len / 2;
    for(i = 0; i < half; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp; 
    }
    //printf("%s\n", str);
}

/*
 * 交换函数
 */
void Swap(char* ch1, char* ch2) {
    char temp;      //用于交换

    temp = *ch1;
    *ch1 = *ch2;
    *ch2 = temp;
}

/*
 * 循环右移函数
 * 算法思路：
 *   1.先将字符串右边 offset 个字符串进行翻转；
 *   2.再将字符串左边剩余字符串进行翻转;
 *   3.最后将整个字符串进行翻转.
 * 代码以字符串“abcXYZ789”循环左移3位进行注释
 */
void RotateRight(char* str, int offset) {
    int len = strlen(str);
    int i, half;    //用于循环控制
    int mid;        //用于前后字符串分界
    
    /* 对超过字符串长度的offset进行取余处理 */
    offset = offset % len;
    
    /* 刚好移动字符串长度的整数倍不需要处理 */
    if(0 == offset) {
        return;
    }
    
    mid = len - offset -1;  //这里的mid是字符串前面部分最后一个字符的索引

    /* 先将右边3个字符串进行翻转：abcXYZ789 --> abcXYZ987 */
    half = offset / 2;
    for(i = 0; i < half; i++) {
        Swap(&str[mid + i + 1], &str[len - i - 1]);
    }
    //printf("%s\n", str);
    
    /* 再将左边剩余字符串进行翻转：abcXYZ987 --> ZYXcba987 */
    half = (len - offset) / 2;
    for(i = 0; i < half; i++) {
        Swap(&str[i], &str[mid - i]);
    }
    //printf("%s\n", str);
    
    /* 最后将整个字符串进行翻转：ZYXcba987 --> 789abcXYZ */
    half = len / 2;
    for(i = 0; i < half; i++) {
        Swap(&str[i], &str[len - i - 1]);
    }
    //printf("%s\n", str);
}

int main()
{
    char str[] = "abcXYZ789";
    int offset;
    printf("原始字符串：%s\n", str);

    printf("输入循环偏移量：");
    scanf("%d", &offset);

    RotateLeft(str, offset);
    printf("循环左移%d位后字符串：%s\n", offset, str);

    RotateRight(str, offset);
    printf("循环右移%d位后字符串：%s\n", offset, str);

    return 0;
}
