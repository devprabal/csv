CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

BUILD_DIR = build

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

all:
	@make clean --no-print-directory
	@make execute --no-print-directory

execute: $(BUILD_DIR)/main
	./$(BUILD_DIR)/main
	
$(BUILD_DIR)/main: $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
