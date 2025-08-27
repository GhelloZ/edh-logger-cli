CC = gcc
AR = ar
CFLAGS = -Wall -I./lib
OBJ = lib/deck.o lib/game.o lib/player.o lib/utils.o lib/stats_calc.o

all: edh

# Build static C library
libliblib.a: $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build Go CLI
edh: libliblib.a
	go build -o edh

clean:
	rm -f $(OBJ) libliblib.a edh

