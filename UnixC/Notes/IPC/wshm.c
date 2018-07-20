/*************************************************************************
    > File Name: wshm.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月20日 星期五 18时22分43秒
    > Description: 利用共享内存发送数据，适用于大数据块传输
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
	printf ("创建共享内存...\n");
	key_t key = ftok (".", 166);
	if (-1 == key){
		perror ("ftok");
		return -1;
	}
	int shmid = shmget (key, 4096, 0666 | IPC_CREAT);
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

	printf ("写数据到共享内存...\n");

	while(1){
		gets_s (shmaddr, 4096);
		if (0 == strcmp (shmaddr, "!")){
			break;
		}
	}

	printf ("卸载共享内存...\n");
	if (-1 == shmdt (shmaddr)) {
		perror ("shmdt");
		return -1;
	}

	printf ("销毁共享内存...\n");
	if (-1 == shmctl (shmid, IPC_RMID, NULL)) {
		perror ("shmctl");
		return -1;
	}

	printf ("一切OK...\n");

	return 0;
}
