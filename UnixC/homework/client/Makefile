CC = gcc
object = client client.o common.o tcpcli.o udpcli.o sha1.o
LFLAG = -lpthread


all: client


client: common.o client.o tcpcli.o udpcli.o sha1.o
	$(CC) common.o client.o tcpcli.o sha1.o udpcli.o -o client $(LFLAG)
client.o: client.c
	$(CC) -c client.c $(LFLAG)
tcpcli.o: tcpcli.c
	$(CC) -c tcpcli.c $(LFLAG)
udpcli.o: udpcli.c
	$(CC) -c udpcli.c $(LFLAG)
sha1.o: sha1.c
	$(CC) -c sha1.c $(LFLAG)
common.o: common.c
	$(CC) -c common.c $(LFLAG)


clean: 
	rm $(object)
