CC=gcc
CFLAGSMAIN=-L./lib/SDL2/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm
CFLAG=-W -Wall -I./lib/SDL2
SRCDIR   = src
LIBDIR   = lib
OBJDIR   = obj
BINDIR   = bin
EXEC     = main
SAVE	 = save

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(LIBDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


all:$(BINDIR) $(OBJDIR) ../$(BINDIR)/$(EXEC) lancement remove

#cree les dossiers obj et bin si ils n'existent pas
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)


../$(BINDIR)/$(EXEC):$(OBJECTS)
	$(CC) -o $(OBJDIR)/$@ $(OBJECTS) $(CFLAGSMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAG)

lancement:
	$(BINDIR)/$(EXEC)

.PHONY:	clean
clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove:clean
	$(rm) $(BINDIR)/$(EXEC)
	@echo "Executable removed!"

val: ../$(BINDIR)/$(EXEC)
	valgrind --leak-check=full $(BINDIR)/$(EXEC)

rmsave:
	$(rm) $(SAVE)/*.json
	@echo "Save removed!"

.PHONY: gdb
gdb:
	$(MAKE) clean
	$(MAKE) CFLAG="$(CFLAG) -g" CFLAGSMAIN="$(CFLAGSMAIN) -g" ../$(BINDIR)/$(EXEC)
	gdb $(BINDIR)/$(EXEC)