CC = gcc

CFLAGS += -I./
CFLAGS += -I./examples/
CFLAGS += -Wall -Wextra -pedantic -std=c11

SRC += $(wildcard *.c)
SRC += $(wildcard examples/*.c)

OBJ += $(SRC:.c=.o)

main.out: $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f main.out
