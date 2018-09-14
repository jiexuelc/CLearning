/*************************************************************************
	> File Name: VariableTypeSize_64.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Tue 11 Sep 2018 10:19:43 AM CST
	> Description: GCC4.2.1下面各变量类型大小
 ************************************************************************/

#include <stdio.h>
#include <string.h>

enum color {
    BLACK = 0,
    YELLOW = 1,
    WHITE,
    BLUE
};  //4 Byte

/*
 * 字节对齐原则：
 *   1.数据类型自身的对齐值:对于char型数据，为1；对于short型，为2，对于int,float型，为4；double类型，为8，单位字节
 *   2.结构体或者类的自身对齐值：其成员中自身对齐值最大的那个值
 *   3.指定对齐值：#pragma pack (value)时的指定对齐值value。
 *   4.数据成员、结构体和类的有效对齐值：自身对齐值和指定对齐值中小的那个值
 * 注意:32位GCC默认指定对齐为4字节,64位GCC默认指定对齐为8字节
 *     VS2010不管是32位还是64位编译器,其默认指定对齐值为结构体或者类的自身对齐值(成员中自身对齐值最大的值)
 */
struct person8 {
    char gender;    //占用2字节，自身1字节
    short age;      //占用2字节
    char party;     //占用4字节，自身1字节
    int height;     //占用4字节
    char name[5];   //占用12字节，自身5字节
    double weight;    //占用8字节
    char addr;      //占用8字节，自身1字节
};  //40 Byte   有效对齐值8

struct person4 {
    char gender;    //占用2字节，自身1字节
    short age;      //占用2字节
    char party;     //占用4字节，自身1字节
    int height;     //占用4字节
};  //12 Byte   有效对齐值4

struct person2 {
    char gender;    //占用2字节，自身1字节
    short age;      //占用2字节
    char party;     //占用2字节，自身1字节
};  //6 Byte   有效对齐值2

struct person1 {
    char gender;    //占用1字节，自身1字节
};  //1 Byte   有效对齐值1

/*
 * #pragma pack(1/2/4/8)修改指定对齐字节数
 * 注意：修改并不一定能奏效，因为还要看结构中成员最大那个对齐值
 *      成员最大对齐值超过指定对齐值，指定才能起作用
 */
#pragma pack(2)
struct Pragma_2
{
    int   a;    //4
    char  b;    //2
    double c;   //8
    char  d;    //2
};  //16 Byte   有效对齐值2

#pragma pack(1)
struct Pragma_1
{
    char  a;    //1
    double b;   //8
    char  c;    //1
};  //10 Byte   有效对齐值1
#pragma pack()

typedef struct IP {
    unsigned char szIP1;    //high
    unsigned char szIP2;
    unsigned char szIP3;
    unsigned char szIP4;
}IP_INT;

typedef union IP_addr {
    IP_INT ip_int;
    int ipv4;
    char type;
}IP_ADDR;   //4 Byte

static double function(int a, float b)
{
    return 0;
}

int main(void)
{
    char szchar = 65;   //1字节
    unsigned char uszchar = 95; //1字节
    
    short sishort = -100;   //2字节
    unsigned short usishort = 100;  //2字节
    
    int iint = -200;    //4字节
    unsigned int uiint= 200;    //4字节
    
    float ffloat = -0.8;    //4字节
    double ddouble = 99.99; //8字节
    
    long llong = 999;   //8字节
    unsigned long ullong = 999; //8字节
    long long lllong = 9999;    //8字节
    
    enum color color1;  //4字节
    union IP_addr pc_ip;    //4字节
    
    char* pchar;    //8字节
    int* pint;  //8字节
    long long* pllint;  //8字节
    double* pdouble;    //8字节
    
    double (*pffun)(int,float); //8字节
    
    /*
     * sizeof计算大小计算整个字符串长度
     * strlen计算截止到'\0'的字符长度
     */
    char str1[] = "12345\0678";     //str1字符串加上结束符'\0'一共占用8字节,'\067'表示一个字符
    char str2[] = "12345\\0678";    //str2字符串加上结束符'\0'一共占用11字节,'\\'表示一个'\'字符
    
    /* ArryNames是一个数组，故sizeof(ArryNames)大小为72字节 */
    char ArryNames[9][8] = {"Tom","Jerry","Ham","Jobs","","Benson","Nick","Evan","jiexue"};
    /* names是一个指针数组，故sizeof(names)大小为72字节 */
    const char* names[9] = {"Tom","Jerry","Ham","Jobs","","Benson","Nick","Evan","jiexue"};
    
    printf("    -----------------------------    \n");
    printf("****在%d位系统上,各类型占用字节数****\n", sizeof(void*) * 8);
    printf("****      GCC Version: %s     ****\n", __VERSION__);
    printf("    -----------------------------    \n");
    printf("-------------基本类型----------------\n");
    printf("char: %d\n", sizeof(szchar));
    printf("unsigned char: %d\n", sizeof(uszchar));
    printf("short: %d\n", sizeof(sishort));
    printf("unsigned short: %d\n", sizeof(usishort));
    printf("int: %d\n", sizeof(iint));
    printf("unsigned int: %d\n", sizeof(uiint));
    printf("float: %d\n", sizeof(ffloat));
    printf("double: %d\n", sizeof(ddouble));
    printf("long: %d\n", sizeof(llong));
    printf("unsigned long: %d\n", sizeof(ullong));
    printf("long long: %d\n", sizeof(lllong));
    
    printf("---------枚举、结构、联合------------\n");
    printf("enum: %d\n", sizeof(enum color));
    printf("struct person8: %d\n", sizeof(struct person8));
    printf("struct person4: %d\n", sizeof(struct person4));
    printf("struct person2: %d\n", sizeof(struct person2));
    printf("struct person1: %d\n", sizeof(struct person1));
    printf("union IPV4: %d\n", sizeof(pc_ip));
    
    printf("--------指针、函数、函数指针---------\n");
    printf("char*: %d\n", sizeof(char*));
    printf("int*: %d\n", sizeof(pint));
    printf("long long*: %d\n", sizeof(pllint));
    printf("double*: %d\n", sizeof(pdouble));
    printf("main: %d\n", sizeof(main)); //标准C禁止这么使用，GCC返回1，VS编译不通过
    printf("function: %d\n", sizeof(function)); //标准C禁止这么使用，GCC返回1，VS编译不通过
    printf("&main: %d\n", sizeof(&main)); //返回8
    printf("&function: %d\n", sizeof(&function));    //返回8
    printf("main(): %d\n", sizeof(main())); //返回4
    printf("function*: %d\n", sizeof(pffun));
    
    printf("---------------字符串----------------\n");
    printf("str1[] = \"12345\\0678\": %d\n",strlen(str1));
    printf("str1[] = \"12345\\0678\": %d\n", sizeof(str1));
    printf("str2[] = \"12345\\\\0678\": %d\n",strlen(str2));
    printf("str2[] = \"12345\\\\0678\": %d\n", sizeof(str2));
    printf("char ArryNames[9][8]: %d\n", sizeof(ArryNames));
    printf("char ArryNames[9][8]: %d\n", strlen(ArryNames[0])); //strlen(ArryNames)会给警告，得到的依然是3
    printf("char* names[9]: %d\n", sizeof(names));
    
    printf("------------修改对齐方式-------------\n");
    printf("struct Pragma_2: %d\n", sizeof(struct Pragma_2));
    printf("struct Pragma_1: %d\n", sizeof(struct Pragma_1));
    
    getchar();
    
    return 0;
}
