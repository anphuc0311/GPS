CC = gcc
CFLAGS = -Wall -Wextra -pedantic
INCLUDE = -Ilib
SRC = lib/gps.c src/output_gps.c
OUTPUT = output_gps

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)

run: $(OUTPUT)
	./$(OUTPUT)

.PHONY: all clean run
