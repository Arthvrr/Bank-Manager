CC = gcc
CFLAGS = -w -pthread -I include
SRC = main.c
TARGET = banque

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)