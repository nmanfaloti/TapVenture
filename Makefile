main: main.o lang.o json.o aff.o button.o combat.o boutique.o
	gcc -o main main.o lang.o json.o aff.o button.o combat.o boutique.o -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm

main.o: main.c
	gcc -o main.o -c main.c -W -Wall

lang.o: traduction/lang.c traduction/lang.h
	gcc -o lang.o -c traduction/lang.c -W -Wall

json.o: option/json.c option/json.h
	gcc -o json.o -c option/json.c -W -Wall

aff.o: affichage/aff.c affichage/aff.h
	gcc -o aff.o -c affichage/aff.c -W -Wall

button.o: button/button.c button/button.h
	gcc -o button.o -c button/button.c -W -Wall

combat.o: combat/combat.c combat/combat.h
	gcc -o combat.o -c combat/combat.c -W -Wall

boutique.o: boutique/boutique.c boutique/boutique.h
	gcc -o boutique.o -c boutique/boutique.c -W -Wall

clean:
	rm -f main *.o