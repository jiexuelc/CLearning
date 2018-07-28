/*************************************************************************
    > File Name: arg.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月27日 星期五 20时19分05秒
    > Description: 线程参数值
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define PAI 3.14159

void* thread_area(void* arg) {
	double r = *(double*)arg;
	*(double*)arg = PAI * r *r;
	return NULL;
}

typedef struct tag_Pyth {
	double a;
	double b;
	double c;
} PYTH, *LPPYTH;

void* thread_pyth(void* arg){
	LPPYTH pyth = (LPPYTH)arg;
	pyth->c = sqrt (pyth->a * pyth->a + pyth->b * pyth->b);
	return;
}

int main(void)
{
	printf ("r = ");
	double rs;
	int error;
	scanf ("%lf", &rs);
	
	pthread_t tid;
	error = pthread_create (&tid, NULL, thread_area, (void*)&rs);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	if ((error = pthread_join (tid, NULL)) != 0) {
		fprintf (stderr, "pthread_join: %s\n", strerror(error));
		return -1;
	}
	
	printf ("s = %g\n", rs);
	
	PYTH pyth;
	printf ("a = ");
	scanf ("%lf", &pyth.a);
	printf ("b = ");
	scanf ("%lf", &pyth.b);
	
	error = pthread_create (&tid, NULL, thread_pyth, (void*)&pyth);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	if ((error = pthread_join (tid, NULL)) != 0) {
		fprintf (stderr, "pthread_join: %s\n", strerror(error));
		return -1;
	}
	
	printf ("c = %g\n", pyth.c);
	
	return 0;
	
}
