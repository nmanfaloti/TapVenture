# Compiler et flags
CC = gcc
CFLAGS = -Wall -g
LIBS = -lcunit

# Nom de l'exécutable et fichiers sources
TARGET = test_goldGainOffline
SRC = test_goldGainOffline.c goldGainOffline.c

# Cible par défaut : construire le binaire de test
all: $(TARGET)

$(TARGET):$(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Exécuter les tests
run:$(TARGET)
	./$(TARGET)

# Nettoyer les fichiers générés
clean:
	rm -f $(TARGET)