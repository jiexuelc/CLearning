/*************************************************************************
	> File Name: PointerArray.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Tue 25 Sep 2018 10:57:26 AM CST
	> Description: 指针与数组
 ************************************************************************/

#include <stdio.h>

int main()
{
    int* a[5];      //a是一个数组，5个元素，每个元素是int＊
    int* f(double); //f是一个函数，形参double,返回类型int*
    int(*p)[5];     //p是一个指针，指向5个元素的int数组
    int x[5]={11,22,33,44,55};  //x是一个5个元素的int数组
    int m;      //m是一个int变量
    int* n;     //n是一个指针，指向int变量
    n = &m;
    int y[6]={12,23,34,45,56,67};
    int z[3][5]={{1,2,3,4},{5,6,7},{8}};
    p = &x;     //只用x表示&x[0]，int*类型，
                //用&x表示数组地址，int(*)[5]类型
                
    int i, j;
    for(i=0; i<5; i++)
        printf("%d ", *(*p+i));//(*p)[i];//x[i]
    printf("\n");
    
    //p = &y;   //&y的类型是int(*)[6]，不一致
    
    p = z;      //数组名z表示z[0]的地址,是一个int[5]的地址
    for(i=0; i<3; i++,printf("\n"))
        for(j=0; j<5; j++)
            printf("%d ", *(*(p+i)+j));     //p[i][j]);//z[i][j]
            
            return 0;
}
