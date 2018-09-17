# Linux中的两个经典宏定义
------

## offsetof

#### offsetof介绍
- 定义：offsetof在linux内核的include/linux/stddef.h中定义。
- 原码实现：
```
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
```
- 作用：获得结构体(TYPE)的变量成员(MEMBER)在此结构体中的偏移量。
- 代码解释：

1. `((TYPE *)0)`   将零转型为TYPE类型指针，即TYPE类型的指针的地址是0。
2. `((TYPE *)0)->MEMBER`   访问结构中的数据成员MEMBER。
3. `&(((TYPE *)0)->MEMBER)`   取出数据成员MEMBER的地址。由于TYPE的地址是0，这里获取到的地址就是MEMBER在TYPE中的偏移。
4. `(size_t)(&(((TYPE*)0)->MEMBER))`   结果转换类型。对于32位系统而言，size_t是unsigned int类型；对于64位系统而言，size_t是unsigned long类型。

#### offsetof使用示例
```c
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
```

#### 输出
```
gender_offset: 0
   age_offset: 2
 party_offset: 4
height_offset: 8
  name_offset: 12
weight_offset: 20
  addr_offset: 28
```

------

## container_of
#### container_of介绍
- 定义：container_of在linux内核的include/linux/kernel.h中定义。
- 原码实现：
```
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
```
- 作用：根据"结构体(type)变量"中的"域成员变量(member)的指针(ptr)"来获取指向整个结构体变量的指针。
- 代码解释：

1. `typeof( ( (type *)0)->member )`   取出member成员的变量类型。
2. `const typeof( ((type *)0)->member ) *__mptr = (ptr)`  定义变量\_\_mptr指针，并将ptr赋值给\_\_mptr。经过这一步，__mptr为member数据类型的常量指针，其指向ptr所指向的地址。
3. `(char *)__mptr`   将__mptr转换为字节型指针。
4. `offsetof(type,member))`   就是获取"member成员"在"结构体type"中的位置偏移。
5. `(char *)__mptr - offsetof(type,member))`  就是用来获取"结构体type"的指针的起始地址（为char *型指针）。
6. `(type *)( (char *)__mptr - offsetof(type,member) )`   就是将"char *类型的结构体type的指针"转换为"type *类型的结构体type的指针"。

## 总结
- 这两个宏对于C++里面class类同样适用
- c++里面使用的时候注意将类成员变量设定为公有，不然不可访问，也就无法操作
------
##### 参考：https://www.cnblogs.com/skywang12345/p/3562146.html