/*************************************************************************
    > File Name: concur.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月27日 星期五 19时53分33秒
    > Description: 线程并发
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* thread_proc(void* arg){
	size_t i;
	for (i = 0; i < 200; i++) {
		printf ("%*d\n", ((size_t)arg + 1) * 4, i + 1);
		usleep (20000);
	}
	return NULL;
}

int main(){
	pthread_t tids[20];
	size_t i;
	int error;
	for (i = 0; i< sizeof(tids) / sizeof(tids[0]); i++) {
		error = pthread_create (&tids[i], NULL, thread_proc, (void*)i);
		if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
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
	
	return -1;
}
