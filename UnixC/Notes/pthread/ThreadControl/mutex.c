/*************************************************************************
    > File Name: mutex.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 08时39分24秒
    > Description: 线程互斥量
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>

unsigned int gcn = 0;

pthread_mutex_t gmutex;

void* thread_proc(void* arg) {
	unsigned int i;
	
	for (i = 0; i < 4000000; i++) {
		pthread_mutex_lock (&gmutex);
		++gcn;
		pthread_mutex_unlock (&gmutex);
	}
	
	return NULL;
}

int main()
{
	size_t i;
	pthread_t tids[2];
	int error;
	
	pthread_mutex_init (&gmutex, NULL);
	
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
	
	pthread_mutex_destroy (&gmutex);
	
	printf ("gcn = %u\n", gcn);
	
	return 0;
}
