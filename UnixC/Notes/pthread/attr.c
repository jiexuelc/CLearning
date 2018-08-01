/*************************************************************************
    > File Name: attr.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月30日 星期一 08时08分29秒
    > Description: 线程属性
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define __USER_GNU

int printattr(pthread_attr_t* attr) {
	printf ("----------线程属性---------\n");	
	
	int error;
	int detachstate;
	if ((error = pthread_attr_getdetachstate (attr, &detachstate)) != 0 ) {
		fprintf (stderr, "pthread_attr_getdetachstate: %s\n", strerror (error));
		return -1;
	}
	printf ("分离状态：  %s\n", (detachstate == PTHREAD_CREATE_DETACHED) ? "分离线程" :
			(detachstate == PTHREAD_CREATE_JOINABLE) ? "可汇合线程" : "未知");
	
	int scope;
	if ((error = pthread_attr_getscope (attr, &scope)) != 0 ) {
		fprintf (stderr, "pthread_attr_getscope: %s\n", strerror (error));
		return -1;
	}
	printf ("竞争范围：  %s\n", (scope == PTHREAD_SCOPE_SYSTEM) ? "系统级竞争" :
			(scope == PTHREAD_SCOPE_PROCESS) ? "进程级竞争" : "未知");
			
	int inheritsched;
	if ((error = pthread_attr_getinheritsched (attr, &inheritsched)) != 0 ) {
		fprintf (stderr, "pthread_attr_getinheritsched: %s\n", strerror (error));
		return -1;
	}
	printf ("继承特性：  %s\n", (inheritsched == PTHREAD_INHERIT_SCHED) ? "继承调用" :
			(inheritsched == PTHREAD_EXPLICIT_SCHED) ? "显式调用" : "未知");
			
	int schedpolicy;
	if ((error = pthread_attr_getschedpolicy (attr, &schedpolicy)) != 0 ) {
		fprintf (stderr, "pthread_attr_getschedpolicy: %s\n", strerror (error));
		return -1;
	}
	printf ("调度策略：  %s\n", (schedpolicy == SCHED_OTHER) ? "普通" :
			(schedpolicy == SCHED_FIFO) ? "先进先出" :
			(schedpolicy == SCHED_RR) ? "轮转" : "未知");
			
	struct sched_param schedparam;
	if ((error = pthread_attr_getschedparam (attr, &schedparam)) != 0 ) {
		fprintf (stderr, "pthread_attr_getschedparam: %s\n", strerror (error));
		return -1;
	}
	printf ("调优先级：  %d\n", schedparam.sched_priority);
	
	size_t guardsize;
	if ((error = pthread_attr_getguardsize (attr, &guardsize)) != 0 ) {
		fprintf (stderr, "pthread_attr_getguardsize: %s\n", strerror (error));
		return -1;
	}
	printf ("栈尾警戒区: %u字节\n", guardsize);
	
	void* stackaddr;
	size_t stacksize;
	if ((error = pthread_attr_getstack (attr, &stackaddr, &stacksize)) != 0 ) {
		fprintf (stderr, "pthread_attr_getstack: %s\n", strerror (error));
		return -1;
	}
	printf ("栈地址：    %p\n", stackaddr);
	printf ("栈大小：    %u\n", stacksize);
	
	printf ("---------------------------\n");
	return 0;
}

void* thread_proc(void* arg) {
	pthread_attr_t attr;
	int error = pthread_getattr_np (pthread_self(), &attr);
	if (0 != error) {
		fprintf (stderr, "pthread_getattr_np: %s\n", strerror (error));
		exit (EXIT_FAILURE);
	}
	
	if (printattr (&attr) < 0)
		exit (EXIT_FAILURE);
	
	exit (EXIT_SUCCESS);
	
	return NULL;
}

int main()
{
	int error;
	pthread_t tid;
	pthread_attr_t attr;
	
	if ((error = pthread_attr_init (&attr)) != 0) {
		fprintf (stderr, "pthread_attr_init: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED)) != 0) {
		fprintf (stderr, "pthread_attr_setdetachstate: %s\n", strerror (error));
		return -1;
	}
	
	/*if ((error = pthread_attr_setscope (&attr, PTHREAD_EXPLICIT_SCHED)) != 0) {
		fprintf (stderr, "pthread_attr_setscope: %s\n", strerror (error));
		return -1;
	}*/
	
	if ((error = pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED)) != 0) {
		fprintf (stderr, "pthread_attr_setinheritsched: %s\n", strerror (error));
		return -1;
	}
	
	if ((error = pthread_attr_setstacksize (&attr, 4096*10)) != 0) {
		fprintf (stderr, "pthread_attr_setstacksize: %s\n", strerror (error));
		return -1;
	}
	
	error = pthread_create (&tid, &attr, thread_proc, NULL);
	if (0 != error) {
		fprintf (stderr, "pthread_create: %s\n", strerror(error));
		return -1;
	}
	
	if ((error = pthread_attr_destroy(&attr)) != 0) {
		fprintf (stderr, "pthread_attr_destroy: %s\n", strerror (error));
		return -1;
	}
	
	pause();	
	
	return -1;	
}
