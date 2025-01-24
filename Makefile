CC=gcc
CFLAGSMAIN=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm
CFLAG=-W -Wall
SRCDIR   = src
LIBDIR   = lib
OBJDIR   = obj
BINDIR   = bin
EXEC     = main

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(LIBDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


all:$(BINDIR) $(OBJDIR) ../$(BINDIR)/$(EXEC)
#cree les dossiers obj et bin si ils n'existent pas
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)


../$(BINDIR)/$(EXEC):$(OBJECTS)
	$(CC) -o $(OBJDIR)/$@ $(OBJECTS) $(CFLAGSMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAG)


.PHONY:	clean
clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove:clean
	$(rm) $(BINDIR)/$(EXEC)
	@echo "Executable removed!"