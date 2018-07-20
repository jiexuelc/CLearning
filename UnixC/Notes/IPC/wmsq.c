/*************************************************************************
    > File Name: wmsq.c
    > Author: jiexuelc
    > Mail: jiexuelc@163.com 
    > Created Time: 2018年07月20日 星期五 20时08分35秒
    > Description: 发送消息队列
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
	printf ("创建消息队列...\n");
	key_t key = ftok (".", 166);
	if (-1 == key){
		perror ("ftok");
		return -1;
	}

	int msqid = msgget (key, 0666 | IPC_CREAT);
	if (-1 == msqid) {
		perror ("msgget");
		return -1;
	}

	printf ("向消息队列发送数据...\n");
	while (1) {
		printf ("> ");

		struct {
			unsigned int mtype;
			char mtext[1024];
		} msgbuf = {188, ""};
		gets_s (msgbuf.mtext, sizeof (msgbuf.mtext));
		
		if (0 == strcmp (msgbuf.mtext, "!")) {
			break;
		}

		if (-1 == msgsnd(msqid, &msgbuf, (strlen (msgbuf.mtext) + 1) * 
			sizeof (msgbuf.mtext[0]), 0)) {
			perror ("msgsnd");
			return -1;
		}
	}

	printf ("销毁消息队列...\n");
	if (-1 == msgctl (msqid, IPC_RMID, NULL)) {
		perror ("msgctl");
		return -1;
	}

	printf ("一切OK...\n");

	return 0;
}
