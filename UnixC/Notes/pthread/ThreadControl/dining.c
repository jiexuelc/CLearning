/*************************************************************************
    > File Name: dining.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年08月02日 星期四 19时24分12秒
    > Description: 线程条件变量，解决哲学家就餐问题
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DINERS 5	//就餐人数

/* 就餐状态 */
typedef enum DiningState {
	THINKING,	//思考中
	HUNGERY,	//正在挨饿
	EATING		//正在吃面条
}	ESTAT;

pthread_mutex_t gmtx = PTHREAD_MUTEX_INITIALIZER;

/* 就餐者条件变量 */
pthread_cond_t gconds[DINERS] = {
	PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER};
	
/* 就餐者姓名 */
const char* gnames[DINERS] = {
	"Tom  ", "Bob  ", "Smith", "Marks", "Jenly"};
	
/* 初始化就餐者状态 */
ESTAT gStats[DINERS] = {
	THINKING, THINKING, THINKING, THINKING, THINKING};

/* 吃面条 */
void eating(int i) {
	int hl = (i + 1) % DINERS; //左邻
	int hr = (i - 1 + DINERS) % DINERS; //右舍
	
	pthread_mutex_lock (&gmtx);
	
	/* 只要左右两边有一个人在吃，睡入条件变量 */
	while (gStats[hl] == EATING || gStats[hr] == EATING) {
		gStats[i] = HUNGERY;
		printf ("%s: Please, I am very hugry !\n", gnames[i]);
		pthread_cond_wait (&gconds[i], &gmtx);
	}
	
	gStats[i] = EATING;
	printf ("%s: The noodles are very delicious!\n", gnames[i]);
	
	usleep ((rand() % 100) * 1000); //模仿吃面条时间
	
	//pthread_mutex_unlock (&gmtx);
}

/* 思考 */
void thinking(int i) {
	int hl = (i + 1) % DINERS; //左邻
	int hr = (i - 1 + DINERS) % DINERS; //右舍
	
	
	//pthread_mutex_lock (&gmtx);
	
	printf ("%s: I will being thinking !\n", gnames[i]);
	gStats[i] = THINKING;
	
	pthread_mutex_unlock (&gmtx); //进入思考状态，放下筷子
	
	pthread_cond_signal (&gconds[hl]); //唤出左边的
	pthread_cond_signal (&gconds[hr]); //唤出右边的
	
	//pthread_mutex_unlock (&gmtx);
	
	usleep ((rand() % 100) * 5000);		//思考时间
}

/* 就餐者线程 */
void* thread_diner(void* arg) {
	int i = (int)arg;
	
	while(1) {
		eating (i);
		thinking (i);
	}
	return NULL;
}

int main()
{
	srand (time (NULL));
	
	pthread_attr_t attr;
	int error;
	if ((error = pthread_attr_init (&attr)) != 0) {
		fprintf (stderr, "pthread_attr_init: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_attr_setdetachstate (&attr,
		PTHREAD_CREATE_DETACHED)) != 0) {
		fprintf (stderr,"pthread_attr_setdetachstate: %s\n",
				strerror (error));
		return -1;
	}
	
	size_t i;
	pthread_t tids[DINERS];
	for (i = 0; i < sizeof (tids) / sizeof (tids[0]); i++) {
		if ((error = pthread_create (&tids[i], &attr, thread_diner,
			(void*)i)) != 0) {
			fprintf (stderr, "pthread_create: %s\n", strerror (error));
			return -1;
		}
	}
	
	printf ("Press any key to exit !\n");
	
	getchar();
	
	return 0;
}
