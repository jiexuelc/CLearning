/*************************************************************************
    > File Name: cond.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月01日 星期三 20时21分53秒
    > Description: 条件变量<上>
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_STOCK 20		//仓库容量

char gStorage[MAX_STOCK];	//仓库
size_t gStock = 0;			//当前库存

pthread_mutex_t gmtx = PTHREAD_MUTEX_INITIALIZER;	//互斥量
pthread_cond_t gFull = PTHREAD_COND_INITIALIZER;	//满仓条件
pthread_cond_t gEmpty = PTHREAD_COND_INITIALIZER;	//空仓条件

/* 显示库存 */
void show(const char* who, const char* op, char prod) {
	printf ("%s: ", who);
	size_t i;
	for (i = 0; i < gStock; i++) {
		printf ("%c", gStorage[i]);
	}
	
	printf ("%s%c\n", op, prod);
}

/* 生产者线程 */
void* thread_producer(void* arg) {
	const char* who = (const char*)arg;
	while(1) {
		pthread_mutex_lock (&gmtx);
		
		if (MAX_STOCK <= gStock) {
			printf ("%s: 满仓!\n", who);
			pthread_cond_wait (&gFull, &gmtx);
		}
		
		char prod = 'A' +rand() % 26;
		show (who, "<-", prod);
		gStorage[gStock++] = prod;
		pthread_cond_signal (&gEmpty);
		
		pthread_mutex_unlock (&gmtx);
		
		usleep ((rand() % 100) * 1000);
	}
	
	return NULL;
}


/* 消费者线程 */
void* thread_customer(void* arg) {
	const char* who = (const char*)arg;
	while(1) {
		pthread_mutex_lock (&gmtx);
		
		if (0 >= gStock) {
			printf ("%s: 空仓!\n", who);
			pthread_cond_wait (&gEmpty, &gmtx);
		}
		
		char prod = gStorage[--gStock];
		show (who, "->", prod);
		pthread_cond_signal (&gFull);
		
		pthread_mutex_unlock (&gmtx);
		
		usleep ((rand() % 100) * 1000);
	}
	
	return NULL;
}

int main(void)
{
	srand (time (NULL));
	
	pthread_attr_t attr;
	int error = pthread_attr_init (&attr);
	if (0 != error) {
		fprintf (stderr, "pthread_attr_init: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_attr_setdetachstate (&attr, 
		PTHREAD_CREATE_DETACHED)) != 0) {
		fprintf (stderr, "pthread_attr_setdetachstate: %s\n",
				strerror (error));
		return -1;
	}
	
	pthread_t tid;
	if ((error = pthread_create (&tid, &attr, thread_producer, "Producer")) != 0) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_create (&tid, &attr, thread_customer, "Customer")) != 0) {
		fprintf (stderr, "pthread_create: %s\n", strerror (error));
		return -1;
	}
	
	printf ("Press any key to exit!\n");
	
	getchar();
	
	return 0;
}
