/** @file shm_communication.c
 *  @note 
 *  @brief 利用共享内存实现大文件在父子进程之间的传输，引入信号量以实现同步机制
 *  
 *  @author jiexue
 *  @date 2019年07月17日
 *  
 *  @note 
 *  
 *  @warning 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#define PATHNAME 		"."		///< 创建信号量和共享内存的key路径
#define PROJECT_SHM_ID 	166		///< 共享内存id
#define PROJECT_SEM_ID 	188 	///< 信号量id
#define BUF_SIZE 		4092 	///< 共享内存buf大小
#define OBJ_PREMS 		(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
								///< IPC访问权限
#define FILENAME		"123.log"
#define COPYNAME		"456.log"

typedef struct
{
	int icnt;				///< buf使用字节数
	char szbuf[BUF_SIZE];	///< 共享内存buf
}SHM_SEG_S;

int semaphore_init(key_t sem_key, int* pisem_id);
int semaphore_p(int isem_id);
int semaphore_v(int isem_id);
int shm_mat(int ishm_id, SHM_SEG_S** ppstshm_addr);


int main(void)
{
	SHM_SEG_S *pstshm_addr = NULL; ///< 共享内存指针
	
	/* 创建信号量id标识 */
	key_t sem_key = ftok(PATHNAME, PROJECT_SEM_ID);
	if(-1 == sem_key)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return 1;
	}
	
	/* 创建共享内存id标识 */
	key_t shm_key = ftok(PATHNAME, PROJECT_SHM_ID);
	if(-1 == shm_key)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return 1;
	}
	
	/* 进程创建 */
	pid_t pid = fork();
	if(0 > pid)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return 1;
	}
	else if(0 == pid) ///< 子进程
	{
		int ibytes = 0;
		/* 获取信号量 */
		int isem_id = semget(sem_key, 0, 0);
		if (-1 == isem_id)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 获取共享内存 */
		int ishm_id = shmget(shm_key, 0, 0);
		if (-1 == ishm_id)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 加载共享内存 */
		if(-1 == shm_mat(ishm_id, &pstshm_addr))
		{
			printf("加载共享内存失败\n");
			return 1;
		}
		
		/* 创建文件 */
		int icopy_file_fd = creat(COPYNAME, 0666 | O_CREAT);
		if(-1 == icopy_file_fd)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		while(1)
		{
			semaphore_p(isem_id); ///< 信号量减一操作，使得父进程阻塞
			if(0 == pstshm_addr->icnt)
			{
				semaphore_v(isem_id); ///< 释放操作，它使父进程信号量变为可用
				break;
			}
			else
			{
				ibytes = write(icopy_file_fd, pstshm_addr->szbuf, pstshm_addr->icnt);
				sleep(1);
				printf("子进程写入%d字节数据\n", ibytes);
				semaphore_v(isem_id); ///< 释放操作，它使父进程信号量变为可用
			}
		}
		
		close(icopy_file_fd);
		
		/* 卸载共享内存 */
		if (-1 == shmdt(pstshm_addr))
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
	}
	else ///< 父进程
	{
		int isem_id = -1;
		
		/* 信号量初始化 */
		if(-1 == semaphore_init(sem_key, &isem_id))
		{
			printf("信号量初始化失败\n");
			return 1;
		}
		/* 创建共享内存 */
		int ishm_id = shmget(shm_key, sizeof(SHM_SEG_S), OBJ_PREMS | IPC_CREAT);
		if(-1 == ishm_id)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 加载共享内存 */
		if(-1 == shm_mat(ishm_id, &pstshm_addr))
		{
			printf("加载共享内存失败\n");
			return 1;
		}
		
		/* 打开文件 */
		int ifile_fd = open(FILENAME, O_RDONLY);
		if(-1 == ifile_fd)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		while(1)
		{
			semaphore_p(isem_id); ///< 信号量减一操作，使得子进程阻塞
			pstshm_addr->icnt = read(ifile_fd, pstshm_addr->szbuf, sizeof(pstshm_addr->szbuf));
			printf("父进程读入%d字节数据\n", pstshm_addr->icnt);
			sleep(1);
			semaphore_v(isem_id); ///< 释放操作，它使信号量变为可用
			if(0 == pstshm_addr->icnt)
			{
				break;
			}
		}
		
		close(ifile_fd);
		
		/* 阻塞等待子进程运行完成 */
		if(-1 == waitpid(pid, NULL, 0))
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 销毁信号量 */
		if (-1 == semctl(isem_id, 0, IPC_RMID))
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 卸载共享内存 */
		if (-1 == shmdt(pstshm_addr))
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		
		/* 销毁共享内存 */
		if (-1 == shmctl(ishm_id, IPC_RMID, NULL))
		{
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
	}
	
	return 0;
}

/**
 *
 *
 */
int semaphore_init(key_t sem_key, int* pisem_id)
{
	
	*pisem_id = semget(sem_key, 1, OBJ_PREMS | IPC_CREAT);
	if(-1 == *pisem_id)
	{
		fprintf(stderr, "%s\n",strerror(errno));
		return -1;
	}
	
	if(-1 == semctl(*pisem_id, 0, SETVAL, 1))
	{
		fprintf(stderr, "%s\n",strerror(errno));
		return -1;
	}
	
	return 0;
}

/**
 *
 *
 */
int semaphore_p(int isem_id)
{
	///< 对信号量做减1操作，即等待P（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; ///< P()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(isem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "%s\n",strerror(errno));
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 *
 *
 */
int semaphore_v(int isem_id)
{
	///< 释放操作，它使信号量变为可用，即发送信号V（sv）
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; ///< V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(isem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "%s\n",strerror(errno));
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 *
 * 加载共享内存
 */
int shm_mat(int ishm_id, SHM_SEG_S** ppstshm_addr)
{
	*ppstshm_addr = shmat(ishm_id, NULL, 0);
	if ((void*)-1 == *ppstshm_addr)
	{
		fprintf(stderr, "%s\n",strerror(errno));
		return -1;
	}
	return 0;
}
