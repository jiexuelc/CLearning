my_sha1: SHA1File.o sha1.o
	gcc SHA1File.o sha1.o -o my_sha1
SHA1File.o: SHA1File.c sha1.h
	gcc -c SHA1File.c
sha1.o: sha1.c sha1.h
	gcc -c sha1.c
