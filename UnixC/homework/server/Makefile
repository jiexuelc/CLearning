CC = gcc
object = server server.o common.o sha1.o
LFLAG = -lpthread


all: server


server: common.o server.o sha1.o
	$(CC) common.o server.o sha1.o -o server $(LFLAG)
server.o: server.c
	$(CC) -c server.c $(LFLAG)
sha1.o: sha1.c
	$(CC) -c sha1.c $(LFLAG)
common.o: common.c
	$(CC) -c common.c $(LFLAG)
#tcpcli.o: tcpcli.c
#	$(CC) -c tcpcli.c $(LFLAG)
#udpcli.o: udpcli.c
#	$(CC) -c udpcli.c $(LFLAG)


clean: 
	rm $(object)
