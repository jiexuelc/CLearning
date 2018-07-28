/*************************************************************************
    > File Name: ret.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月28日 星期六 08时16分28秒
    > Description: 获取线程返回值
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PAI 3.14159

void* thread_area(void* arg) {
	double r = *(double*)arg;
	/*static double s;
	s = PAI * r * r;
	return &s;*/
	double* s = malloc (sizeof (double));
	*s = PAI * r * r;
	return s;
}

int main()
{
	printf ("r = ");
	double r;
	scanf ("%lf", &r);
	
	pthread_t tid;
	int error = pthread_create (&tid, NULL, thread_area, (void*)&r);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	double* s;
	if (0 != pthread_join (tid, (void**)&s)) {
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	printf ("s = %g\n", *s);
	
	free (s);
	
	return 0;
}
