Date: Date.o days.o IsDay.o LeapYear.o
	gcc Date.o days.o IsDay.o LeapYear.o
Date.o: Date.c LeapYear.h IsDay.h days.h
	gcc -c Date.c
IsDay.o: IsDay.c IsDay.h LeapYear.h
	gcc -c IsDay.c
days.o: days.c days.h LeapYear.h IsDay.h
	gcc -c days.c
LeapYear.o: LeapYear.c LeapYear.h
	gcc -c LeapYear.c
