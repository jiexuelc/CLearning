/*************************************************************************
    > File Name: create.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月27日 星期五 19时30分54秒
    > Description: 线程创建
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* thread_proc(void* arg){
	printf ("%lu线程：%s\n", pthread_self(), (char*)arg);
	return NULL;
}

int main(){
	pthread_t tid;
	int error = pthread_create (&tid, NULL, thread_proc, "I am child process!");
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	printf ("%lu线程:我是主线程，我创建了子线程%lu\n", pthread_self(), tid);

	sleep (1);
	return -1;
}
