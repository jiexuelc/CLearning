/*************************************************************************
    > File Name: deadlock.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 18时51分29秒
    > Description: 线程死锁
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t gmtxa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gmtxb = PTHREAD_MUTEX_INITIALIZER;

void* thread_proc1(void* arg) {
	pthread_t tid = pthread_self();
	
	printf ("%lu线程：等待A...\n");
	pthread_mutex_lock (&gmtxa);
	printf ("%lu线程：获得A...\n");
	
	sleep (1);
	
	printf ("%lu线程：等待B...\n");
	pthread_mutex_lock (&gmtxb);
	printf ("%lu线程：获得B...\n");
	
	usleep (500000);
	
	pthread_mutex_unlock(&gmtxb);
	printf ("%lu线程：释放B...\n");
	
	pthread_mutex_unlock(&gmtxa);
	printf ("%lu线程：释放A...\n");
	
	return NULL;
}

void* thread_proc2(void* arg) {
	pthread_t tid = pthread_self();
	
	printf ("%lu线程：等待B...\n");
	pthread_mutex_lock (&gmtxb);
	printf ("%lu线程：获得B...\n");
	
	sleep (1);
	
	printf ("%lu线程：等待A...\n");
	pthread_mutex_lock (&gmtxa);
	printf ("%lu线程：获得A...\n");
	
	usleep (500000);
	
	pthread_mutex_unlock(&gmtxa);
	printf ("%lu线程：释放A...\n");
	
	pthread_mutex_unlock(&gmtxb);
	printf ("%lu线程：释放b...\n");
	
	return NULL;
}

int main()
{
	pthread_t tids[2];
	int error = pthread_create (&tids[0], NULL, thread_proc1, NULL);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	error = pthread_create (&tids[1], NULL, thread_proc2, NULL);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	int i;
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		error = pthread_join (tids[i], NULL);
		if (0 != error) {
			fprintf (stderr, "pthread_join: %s\n", strerror (error));
			return -1;
		}
	}
	
	printf ("一切OK...\n");
	return 0;
}
