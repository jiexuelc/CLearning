/*************************************************************************
	> File Name: OffsetofStruct.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: Mon 17 Sep 2018 02:30:27 PM CST
	> Description: 用Linux中offsetof宏计算结构体的成员的偏移量，从而得知系统如何将其对齐
 ************************************************************************/

#include <stdio.h>

#define OffsetOf(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/* 以下为32位GNU4.8.4下的测试结果,不同平台不同编译器可能有细微差别 */

struct person {
    char gender;    //占用2字节，自身1字节
    short age;      //占用2字节
    char party;     //占用4字节，自身1字节
    int height;     //占用4字节
    char name[5];   //占用8字节，自身5字节
    double weight;    //占用8字节
    char addr;      //占用4字节，自身1字节
};  //32 Byte   有效对齐值4

int main(void)
{
    int gender_offset, age_offset, party_offset, height_offset;
    int name_offset, weight_offset, addr_offset;
    
    gender_offset = OffsetOf(struct person, gender);
    age_offset = OffsetOf(struct person, age);
    party_offset = OffsetOf(struct person, party);
    height_offset = OffsetOf(struct person, height);
    name_offset = OffsetOf(struct person, name);
    weight_offset = OffsetOf(struct person, weight);
    addr_offset = OffsetOf(struct person, addr);
    
    printf("gender_offset: %d\n", gender_offset);   //0
    printf("   age_offset: %d\n", age_offset);      //2
    printf(" party_offset: %d\n", party_offset);    //4
    printf("height_offset: %d\n", height_offset);   //8
    printf("  name_offset: %d\n", name_offset);     //12
    printf("weight_offset: %d\n", weight_offset);   //20
    printf("  addr_offset: %d\n", addr_offset);     //28
    
    return 0;
}