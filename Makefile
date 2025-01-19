# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable
TARGET = bin/minesweeper

# Source files
SRCS = src/main.c src/cli/start.c src/cli/play.c src/cli/utils.c src/game/game_logic.c src/game/game_filedb.c

# Object files (derived from SRCS)
OBJS = $(SRCS:src/%.c=bin/%.o)

# Test source files
TEST_SRCS = $(filter-out src/main.c, $(SRCS))

# Test object files
TEST_OBJS = $(TEST_SRCS:src/%.c=bin/%.o)

# Default target
all: build_dir $(TARGET)

# Build directory
build_dir:
	mkdir -p bin/cli bin/game

# Compile the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile object files
bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf bin

# Unit tests target
test: build_dir bin/test_main.o $(TEST_OBJS)
	$(CC) $(CFLAGS) -o bin/test_runner bin/test_main.o $(TEST_OBJS)
	./bin/test_runner

# Compile test main file
bin/test_main.o: tests/main.c
	$(CC) $(CFLAGS) -I src -c tests/main.c -o bin/test_main.o