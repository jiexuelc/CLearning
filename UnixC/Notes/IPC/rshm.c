/*************************************************************************
    > File Name: rshm.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月20日 星期五 18时22分43秒
    > Description: 利用共享内存接收数据，适用于大数据块传输
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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
	printf ("获取共享内存...\n");
	key_t key = ftok (".",166);
	if (-1 == key){
		perror ("ftok");
		return -1;
	}
	int shmid = shmget (key, 0, 0);
	if (-1 == shmid) {
		perror ("shmget");
		return -1;
	}

	printf ("加载共享内存...\n");
	void* shmaddr = shmat (shmid, NULL, 0);
	if ((void*)-1 == shmaddr) {
		perror ("shmat");
		return -1;
	}

	printf ("读取共享内存...\n");
	char c;
	while(1){
		printf ("按回车读取，“!”退出...");
		char c = getchar();
		printf ("%s\n", (char*)shmaddr);
		if (c == '!') break;
	}

	printf ("卸载共享内存...\n");
	if (-1 == shmdt (shmaddr)) {
		perror ("shmdt");
		return -1;
	}

	printf ("一切OK...\n");

	return 0;
}
