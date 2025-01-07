CC = gcc
CFLAGS = -w -pthread -I include
SRC = main.c
TARGET = banque

# Cible par défaut : créer et exécuter l'exécutable
all: $(TARGET)
	./$(TARGET) $(A) $(C)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)