CC = gcc
LFLAG = -lpthread

all: client server

server:
	$(CC) server.c -o server $(LFLAG)
client: 
	$(CC) client.c -o client $(LFLAG)

clean:
	rm client
	rm server
