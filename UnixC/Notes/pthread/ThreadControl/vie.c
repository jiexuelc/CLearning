/*************************************************************************
    > File Name: vie.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 08时12分24秒
    > Description: 线程竞争
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>

unsigned int gcn = 0;

void* thread_proc(void* arg) {
	unsigned int i;
	for (i = 0; i < 400000000; i++) {
		++gcn;
	}
	
	return NULL;
}

int main()
{
	size_t i;
	pthread_t tids[2];
	int error;
	
	for (i = 0; i < sizeof(tids) / sizeof(tids[0]); i++) {
		if ((error = pthread_create (&tids[i], NULL, thread_proc, NULL)) != 0) {
			fprintf (stderr, "pthread_create: %s\n", strerror (error));
			return -1;
		}
	}
		
	for (i = 0; i < sizeof(tids) / sizeof(tids[0]); i++) {
		if ((error = pthread_join (tids[i], NULL)) != 0) {
			fprintf (stderr, "pthread_join: %s\n", strerror (error));
			return -1;
		}
	}
	
	printf ("gcn = %u\n", gcn);
	
	return 0;
}
