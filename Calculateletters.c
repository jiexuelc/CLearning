#include <stdio.h>

int main()
{
    int ch,num=0;
    while((ch=getchar())!='\n')
    {
        if((ch>='a' && ch  <= 'z')||(ch>='A' && ch  <= 'Z'))
            num++;
        //else if(ch=='\n') break;
    }
    printf("��ĸ%d��\n",num);
    return 0;
}