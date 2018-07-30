/*************************************************************************
    > File Name: cancel.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月28日 星期六 20时24分04秒
    > Description: 取消线程
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void eclaps(void) {
	size_t i;
	for (i = 0; i < 800000000; i++) ;
}

void* thread_proc(void* arg) {
	/*
	int error = pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL);
	if (0 != error ) {
		fprintf (stderr, "pthread_setcancelstate: %s\n", strerror (error));
		exit (EXIT_FAILURE);
	}*/
	
	int error = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if (0 != error ) {
		fprintf (stderr, "pthread_setcanceltype: %s\n", strerror (error));
		exit (EXIT_FAILURE);
	}
	
	while (1) {
		printf ("线程运行中...\n");
		eclaps();
	}
	
	return NULL;
}

int main()
{
	setbuf (stdout, NULL);
	printf ("按 <任意键>取消线程...\n");
	
	pthread_t tid;
	int error = pthread_create (&tid, NULL, thread_proc, NULL);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	getchar();
	
	if ((error = pthread_cancel (tid)) != 0) {
		fprintf (stderr, "pthread_cancel: %s\n", strerror(error));
		return -1;
	}
	
	printf ("成功发送取消线程请求，等待线程%lu终止...\n", tid);
	
	if ((error = pthread_join (tid, NULL)) != 0) {
		fprintf (stderr, "pthread_cancel: %s\n", strerror(error));
		return -1;
	}
	printf ("线程已经终止...\n");
	
	return 0;
}
