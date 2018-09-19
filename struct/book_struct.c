/*************************************************************************
	> File Name: structure.c
	> Author: jiexue
	> Mail: jiexuelc@163.com
	> Created Time: 2018年04月08日 星期日 15时57分41秒
	> 描述: 将十本书籍按照价格由高到低输出，再按照库存量由低到高输出
 ************************************************************************/

#include<stdio.h>

typedef struct book{
    char name[30];
    double price;
    int amount;
}book;

int main()
{
    book catalog[10]={
        {"语文",39.9,50},
        {"数学",40.9,45},
        {"英语",59.8,78},
        {"俄语",54.8,101},
        {"物理",49.9,120},
        {"化学",69.8,65},
        {"生物",55.9,59},
        {"政治",36.9,68},
        {"地理",64.8,35},
        {"历史",46.6,53}
    };
    book temp;
    int i,j;

    printf("****************************\n");
    printf("书籍默认顺序：\n");
    for(i=0;i<10;i++){
        printf("书名:%s 价格:%.1f 库存量:%d\n",catalog[i].name,catalog[i].price,catalog[i].amount);
    }
    
    printf("****************************\n");
    printf("按照书籍价格由高到低排序为：\n");
    for(i=0;i<10;i++){
        for(j=i+1;j<10;j++){
            if(catalog[i].price<catalog[j].price){
                temp = catalog[i];
                catalog[i] = catalog[j];
                catalog[j] = temp;
            }
        }
    }    
    for(i=0;i<10;i++){
        printf("书名:%s 价格:%.1f 库存量:%d\n",catalog[i].name,catalog[i].price,catalog[i].amount);
    }

    printf("****************************\n");
    printf("按照书籍库存量由低到高排序为：\n");
    for(i=0;i<10;i++){
        for(j=i+1;j<10;j++){
            if(catalog[i].amount > catalog[j].amount){
                temp = catalog[i];
                catalog[i] = catalog[j];
                catalog[j] = temp;
            }
        }
    }    
    for(i=0;i<10;i++){
        printf("书名:%s 价格:%.1f 库存量:%d\n",catalog[i].name,catalog[i].price,catalog[i].amount);
    }

    return 0;
}
