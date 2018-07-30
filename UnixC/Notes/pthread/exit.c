/*************************************************************************
    > File Name: exit.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月28日 星期六 19时42分46秒
    > Description: 线程退出
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define PAI 3.14159

void* thread_area(void* arg){
	double r = *(double*)arg;
	double* s = malloc (sizeof (double));
	//exit (0);//终止整个进程
	*s = PAI * r * r;
	pthread_exit(s);
	*s = 2 * PAI * r;
	return s;
}

int main()
{
	printf ("r = ");
	
	double r;
	scanf ("%lf", &r);
	
	pthread_t tid;
	int error = pthread_create (&tid, NULL, thread_area, (void*)&r);
	if (0 != error){
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	double* s;
	if ((error = pthread_join (tid, (void**)&s)) != 0) {
		fprintf (stderr, "pthread_join: %s\n", strerror(error));
		return -1;
	}
	printf ("s = %g\n", *s);
	free(s);
	
	sleep (2);
	
	return 0;
}

