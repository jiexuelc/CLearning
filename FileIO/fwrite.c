/*************************************************************************
	> File Name: fwrite.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Tue 25 Sep 2018 08:18:02 PM CST
	> Description: fwrite函数使用，fwrite(内存地址，每个元素字节数，元素个数，文件)
        size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
 ************************************************************************/

#include <stdio.h>

typedef struct person{
    char name[20];  //20b
    char gender;    //1b
    int age;    //4b+空3b
    double salary;  //8b
}person;        //共36bytes

int main()
{
    person a[5] = {{"Tom", 'M', 18, 3500},
                   {"Jack", 'M', 20, 4500},
                   {"Toly", 'M', 21, 4000},
                   {"Jane", 'F', 19, 3600},
                   {"Jery", 'F', 24, 6000}};
    
    FILE* fp = fopen("person.dat", "wb");
    if(NULL == fp){
        printf("打开person.dat文件失败\n");
        return 1;
    }
    
    int b[10] = {11,22,33,44,55,66,77,88,99,1010};
    short s = 12345;
    fwrite(a, sizeof(person), 5, fp);
    fwrite(b, sizeof(int), 10, fp);
    fwrite(&s, sizeof(short), 1, fp);
    fclose(fp);
    
    return 0;
}

