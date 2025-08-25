# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Ilog
LDFLAGS = -lsqlite3

# Source files
SRCS = main.c stats_calc.c $(wildcard log/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = edh

# Default target
all: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

