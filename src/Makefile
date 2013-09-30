

CC=gcc
CFLAGS=-Wall -O2 -I../include
DEPS = statfuncs.h infuncs.h

all: qstats

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

qstats: qstats.o statfuncs.o infuncs.o
	$(CC) -o qstats qstats.o statfuncs.o infuncs.o $(CFLAGS)

clean:
	rm ./*.o
