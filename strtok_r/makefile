main: main.o gets_s.o strtok_r.o
	gcc main.o gets_s.o strtok_r.o -o main
main.o: main.c gets_s.h strtok_r.h
	gcc -c main.c
gets_s.o: gets_s.c gets_s.h
	gcc -c gets_s.c
strtok.o: strtok_r.o strtok_r.h
	gcc -c strtok_r.c
