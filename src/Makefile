

CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wwrite-strings -Wundef -Wmissing-noreturn -Wstrict-prototypes -ansi -pedantic -O2 -I../include
DEPS = statfuncs.h infuncs.h graphfuncs.h qstats.h

all: qstats

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

qstats: qstats.o statfuncs.o infuncs.o graphfuncs.o
	$(CC) -o qstats qstats.o statfuncs.o infuncs.o graphfuncs.o $(CFLAGS)

clean:
	rm -f ./*.o
