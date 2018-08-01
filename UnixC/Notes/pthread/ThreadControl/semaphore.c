/*************************************************************************
    > File Name: semaphore.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 18时12分15秒
    > Description: 信号量
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

sem_t gsem;

unsigned int gcnt = 0;

void* thread_proc(void* arg) {
	unsigned int i;
	for (i = 0; i < 400000; i++) {
		sem_wait (&gsem);
		++gcnt;
		sem_post (&gsem);
	}
	
	return NULL;
}	

int main()
{
	int i;
	pthread_t tids[2];
	int error;
	
	if (-1 == sem_init (&gsem, 0, 1)) {
		fprintf (stderr, "sem_init: %s\n", strerror (errno));
		return -1;
	}
	
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		error = pthread_create (&tids[i], NULL, thread_proc, NULL);
		if (0 != error) {
			fprintf (stderr, "pthread_create: %s\n", strerror(error));
			return -1;
		}
	}
	
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		error = pthread_join (tids[i], NULL);
		if (0 != error) {
			fprintf (stderr, "pthread_join: %s\n", strerror(error));
			return -1;
		}
	}
	
	if (-1 == sem_destroy (&gsem)) {
		fprintf (stderr, "sem_destory: %s\n", strerror (errno));
		return -1;
	}
	
	printf ("gcn = %u\n", gcnt);
	
	return 0;
}
