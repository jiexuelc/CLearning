/*	@file GetDynAPIAddr.c
 *	@note 
 *	@brief	获取c标准库里的系统函数地址：例如read、write等
 *
 *	@author		lichao
 *	@date		Oct 10, 2019
 *
 *	@note 历史记录：
 *	@note Oct 10, 2019 V1.0.0 新建
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STD_LIBC "/lib/x86_64-linux-gnu/libc.so.6"	//< c标准库函数动态库
//#define STD_LIBC "/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2"

typedef ssize_t (*pFUN)(int, void*, size_t);

char buf[1024];
int bytes = 0;

int Print(void)
{
	memset(buf, 0, 1024);
	bytes = read(STDIN_FILENO, buf, 1024);
	write(STDERR_FILENO, buf, bytes);
	return 0;
}

int main(void)
{
	Print();
	void* pLibc = dlopen (STD_LIBC, RTLD_NOW);		//< 立即加载动态库
	printf("pLibc:	%p\n", pLibc);
	
	void* pRead = dlsym (pLibc, "read");
	printf("pRead:	%p\n", pRead);
	
	pFUN pWrite = (pFUN)dlsym (pLibc, "write");		//< 动态库函数获取
	printf("pWrite:	%p\n", pWrite);
	pWrite(STDERR_FILENO, buf, bytes);
	
	dlclose (pLibc);	//< 卸载动态库
	return 0;
}
