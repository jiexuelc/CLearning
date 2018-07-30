/*************************************************************************
    > File Name: detach.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月28日 星期六 20时00分17秒
    > Description: 异步方式执行线程
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>

void* thread_proc(void* arg) {
	int i;
	for (i = 0; i < 200; i++) {
		putchar('-');
		usleep (10000);
	}
	return NULL;
}

int main()
{
	setbuf (stdout, NULL);
	
	pthread_t tid;
	
	int error = pthread_create (&tid, NULL, thread_proc, NULL);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	if (0 != pthread_detach (tid)) {
		(stderr, "pthread_join: %s\n", strerror (error));
		return -1;
	}
	/*
	if ((error = pthread_join (tid, NULL)) != 0) {
		fprintf (stderr, "pthread_join: %s\n", strerror (error));
		return -1;
	}*/
	int i;
	for (i = 0; i < 200; i++) {
		putchar('+');
		usleep(20000);
	}
	
	printf("END!\n");
	return 0;
}
