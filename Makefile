CC = gcc
CFLAGS = -w -pthread -I include
SRC = main.c
TARGET = banque

# Cible par défaut : compiler
all: $(TARGET)

# Cible pour exécuter avec un argument spécifique
run-%: $(TARGET)
	./$(TARGET) $*

# Compilation
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)