my_md5: mddriver.o md5.o
	gcc mddriver.o md5.o -o my_md5
mddriver.o: mddriver.c global.h md5.h
	gcc -c mddriver.c
md5.o: md5.c global.h md5.h
	gcc -c md5.c
