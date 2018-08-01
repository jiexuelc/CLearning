/*************************************************************************
    > File Name: SemPool.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 19时17分03秒
    > Description: 信号池
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_CONNS 5		//最大连接数
#define MAX_USERS 50 	//最大用户数

sem_t gsem;

void* thread_user(void* arg) {
	pthread_t tid = pthread_self ();
	
	int sval;
	if (-1 == sem_getvalue (&gsem, &sval)) {
		fprintf(stderr, " sem_getvalue: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	printf ("%lu线程：等待数据库连接(还剩%d个空闲连接)...\n", tid, sval);
	
	if (-1 == sem_wait (&gsem)) {
		fprintf(stderr, " sem_wait: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	if (-1 == sem_getvalue (&gsem, &sval)) {
		fprintf(stderr, " sem_getvalue: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	printf ("%lu线程：获得数据库连接(还剩%d个空闲连接)...\n", tid, sval);
	
	usleep ((rand () % 100) * 10000);
	
	if (-1 == sem_post (&gsem)) {
		fprintf(stderr, " sem_post: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	if (-1 == sem_getvalue (&gsem, &sval)) {
		fprintf(stderr, " sem_getvalue: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	printf ("%lu线程：释放数据库连接(还剩%d个空闲连接)...\n", tid, sval);
}	

int main(void)
{
	size_t i;
	pthread_t tids[MAX_USERS];
	int error;
	
	if (-1 == sem_init (&gsem, 0, MAX_CONNS)) {
		fprintf (stderr, "sem_init: %s\n", strerror (error));
		return -1;
	}
	
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		error = pthread_create (&tids[i], NULL, thread_user, NULL);
		if (0 != error) {
			fprintf (stderr, "pthread_create: %s\n", strerror (error));
			return -1;
		}
	}
	
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		error = pthread_join (tids[i], NULL);
		if (0 != error) {
			fprintf (stderr, "pthread_join: %s\n", strerror (error));
			return -1;
		}
	}
	
	if (-1 == sem_destroy (&gsem)) {
		fprintf (stderr, "sem_desdory: %s\n", strerror (error));
		return -1;
	}
	
	return 0;
}
