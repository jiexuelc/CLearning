/*************************************************************************
    > File Name: equal.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月28日 星期六 19时25分50秒
    > Description: 比较两个线程号是否相等
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>

pthread_t g_main;

void* ismain(void* arg){
	if (0 != pthread_equal (pthread_self(), g_main))
		printf ("%lu:我是主线程！\n", pthread_self());
	else
		printf ("%lu:我不是主线程！\n", pthread_self());
	
	return NULL;

}

int main()
{
	g_main = pthread_self();
	
	ismain(NULL);
	
	pthread_t tid;
	int error = pthread_create (&tid, NULL, ismain, NULL);
	if (0 != error){
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_join (tid, NULL)) != 0) {
		fprintf (stderr, "pthread_join: %s\n", strerror (error));
		return -1;
	}
	
	return 0;
}
