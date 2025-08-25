CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lsqlite3

# source files
SRCS = main.c log_utils.c stats_calc.c
# object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# output binary name
TARGET = edh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
