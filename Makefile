main: main.o lang.o json.o
	gcc -o main main.o lang.o json.o -lSDL2main -lSDL2

main.o: main.c
	gcc -o main.o -c main.c -W -Wall

lang.o: traduction/lang.c traduction/lang.h
	gcc -o lang.o -c traduction/lang.c -W -Wall

json.o: option/json.c option/json.h
	gcc -o json.o -c option/json.c -W -Wall

clean:
	rm -f main *.o

