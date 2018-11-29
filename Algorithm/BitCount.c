/*************************************************************************
    > File Name: BitCount.c
    > Author: jiexue
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月29日 星期日 14时52分36秒
    > Description: 求二进制数中1的个数
 ************************************************************************/
typedef unsigned int UINT;
/* 解法一 */
/* 循环除2取模，判断余数是否为1
 */
UINT Count1(UINT num) {
	UINT cnt = 0;
	while(num) {
		if (num % 2 == 1)
			cnt++;
		num = num / 2;
	}
	return cnt;
}

/* 解法二 */
/* 循环将最后一位与0x01相与
 */
UINT Count2(UINT num) {
	UINT cnt = 0;
	while(num) {
		cnt += num & 0x01;
		num >>= 1;
	}
	return cnt;
}

/* 解法三 */
/* 每次循环去掉最右边的1
 * 算法复杂度只与二进制数中1的个数有关
 */
UINT Count3(UINT num) {
	UINT cnt = 0;
	while(num) {
		num &= (num -1);
		cnt++;
	}
	return cnt;
}

/* 解法四 */
/* 查表法：以空间复杂度换取时间
 * 将对应的每一个数的1的个数直接存在数组中，用的时候直接查表得到
 */
UINT Count4(UINT num) {
	unsigned char countTable[256] =
	{
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
	};
	return (countTable[num & 0xff] + countTable[(num >> 8) & 0xff] +
			countTable[(num >> 16) & 0xff] + countTable[(num >> 24) & 0xff]);
}

#include<stdio.h>

UINT main()
{
	UINT num = 0xffffffff;
	
	printf ("解法1：%d\n", Count1(num));
	printf ("解法2：%d\n", Count2(num));
	printf ("解法3：%d\n", Count3(num));
	printf ("解法4：%d\n", Count4(num));
	
	return 0;
}
