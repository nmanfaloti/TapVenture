CC=gcc
CFLAGSMAIN=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm
CFLAG=-W -Wall
SRCDIR   = src
LIBDIR   = lib
OBJDIR   = obj
BINDIR   = bin
BINDIREXEC = ../$(BINDIR)
EXEC     = main

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(LIBDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIREXEC)/$(EXEC):$(OBJECTS)
	$(CC) -o $(OBJDIR)/$@ $(OBJECTS) $(CFLAGSMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAG)


.PHONY:	clean
clean:
	$(rm) $(OBJDIR)/*.o
	@echo "Cleanup complete!"

.PHONY: remove
remove:clean
	$(rm) $(BINDIR)/$(EXEC)
	@echo "Executable removed!"