main: main.o lang.o
	gcc -o main main.o lang.o

main.o: main.c
	gcc -o main.o -c main.c -W -Wall

lang.o: traduction/lang.c traduction/lang.h
	gcc -o lang.o -c traduction/lang.c -W -Wall

clean:
	rm -f main main.o lang.o