/*************************************************************************
    > File Name: loccli.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月24日 星期二 08时12分23秒
    > Description: 自己写的gets_s字符串获取函数，防止越界
 ************************************************************************/

#include <stdio.h>
#include <string.h>

char *gets_s(char *str, size_t num)
{
    if (0 != fgets(str, num, stdin))
    {
        size_t len = strlen(str);
        if (len > 0 && str[len-1] == '\n')
            str[len-1] = '\0';
        return str;
    }
    return 0;
}

int main()
{
	char buf[1024];
	
	printf("获取字符串数据...\n");
	while (1) {		
		printf ("> ");
		gets_s (buf, sizeof (buf));
		
		//退出循环
		if (0 == strcmp (buf,"!")) {
			break;
		}
		
		printf ("< %s\n", buf);
	}

	return 0;
}
