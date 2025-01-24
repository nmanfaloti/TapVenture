CC=gcc -o
CFLAGSMAIN=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm
CFLAG=-W -Wall
SRCDIR   = src
LIBDIR   = lib
EXEC     = main
OBJ	  = main.o lang.o json.o aff.o button.o combat.o boutique.o heros.o

$(EXEC):$(OBJ)
	$(CC) $(OBJ) $(CFLAGSMAIN)

%.o:$(SRCDIR)/%.c $(LIBDIR)/%.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(EXEC).o:$(SRCDIR)/$(EXEC).c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f main *.o