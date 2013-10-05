

CC=gcc
CFLAGS=-Wall -O2 -I../include
DEPS = statfuncs.h infuncs.h graphfuncs.h

all: qstats

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

qstats: qstats.o statfuncs.o infuncs.o graphfuncs.o
	$(CC) -o qstats qstats.o statfuncs.o infuncs.o graphfuncs.o $(CFLAGS)

clean:
	rm ./*.o
