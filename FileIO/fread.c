/*************************************************************************
	> File Name: fread.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Wed 26 Sep 2018 10:41:40 AM CST
	> Description: fread函数使用举例　fread(内存地址，每个元素字节数，元素个数，文件)
                size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
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
    person a[5] = {};
    int b[10] = {};
    short s = 0;
    
    FILE* fp = fopen("person.dat", "rb");
    if(NULL == fp){
        printf("打开person.dat文件失败\n");
        return 1;
    }
    
    fread(a, sizeof(person), 5, fp);
    fread(b, sizeof(int), 10, fp);
    fread(&s, sizeof(short), 1, fp);
    fclose(fp);
    
    int i;
    for(i=0; i<5; i++){
        printf("%s:%s,%d,%g\n",
               a[i].name, a[i].gender=='M'?"男":"女",
               a[i].age, a[i].salary);
    }
    for(i=0; i<10; i++)
        printf("%d ", b[i]);
    printf("\n");
    printf("%hd\n", s);
    
    return 0;
}

