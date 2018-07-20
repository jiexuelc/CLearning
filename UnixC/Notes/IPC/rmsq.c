/*************************************************************************
    > File Name: rmsq.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月20日 星期五 20时08分35秒
    > Description: 接收消息队列
 ************************************************************************/

#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char *gets_s(char *str, size_t num)
{
    if (0 != fgets(str, num, stdin))
    {
        size_t len = strlen(str);
        if (len > 0 && str[len-1] == '\n')
            str[len-1] = '\0';
        return str;
    }
    return 0;
}

int main()
{
	printf ("获取消息队列...\n");
	key_t key = ftok (".", 166);
	if (-1 == key){
		perror ("ftok");
		return -1;
	}

	int msqid = msgget (key, 0);
	if (-1 == msqid) {
		perror ("msgget");
		return -1;
	}

	printf ("接收消息队列数据...\n");
	while (1) {

		struct {
			unsigned int mtype;
			char mtext[1024];
		} msgbuf = { };
		ssize_t msgsz = msgrcv (msqid, &msgbuf, sizeof (msgbuf.mtext) - 
				sizeof (msgbuf.mtext[0]), 188, MSG_NOERROR);
		if (-1 ==  msgsz) {
			if (EIDRM == errno) {
				printf ("消息队列已经销毁.....\n");
				break;
			}
			else if (ENOMSG == errno) {
				printf ("现在没有消息....\n");
				sleep(1);
			}
			else {
				perror ("msgrcv");
				return -1;
			}
		}
		else {
			printf ("%04d < %s\n", msgbuf.mtype, msgbuf.mtext);
		}
	}

	printf ("一切OK...\n");

	return 0;
}
