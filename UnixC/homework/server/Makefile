CC = gcc
object = server server.o common.o sha1.o udpser.o tcpser.o
LFLAG = -lpthread


all: server


server: server.o common.o udpser.o tcpser.o sha1.o
	$(CC) server.o common.o sha1.o udpser.o tcpser.o -o server $(LFLAG)
server.o: server.c
	$(CC) -c server.c $(LFLAG)
common.o: common.c
	$(CC) -c common.c $(LFLAG)
sha1.o: sha1.c
	$(CC) -c sha1.c $(LFLAG)
tcpser.o: tcpser.c
	$(CC) -c tcpser.c $(LFLAG)
udpser.o: udpser.c
	$(CC) -c udpser.c $(LFLAG)


clean: 
	rm $(object)
