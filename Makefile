
PREFIX=/usr/local
MANPREFIX=/usr/local/share/
BUILTDIR=./bin/

all: builtdir
	cd ./src && make
	mv ./src/qstats ./bin/

builtdir:
	mkdir -p $(BUILTDIR)

clean:
	cd ./src && make clean
	rm -f ./bin/qstats
	rm -rf ./bin/

install: all
	cp ./bin/qstats $(PREFIX)/bin
	cp ./doc/qstats.1 $(MANPREFIX)/man/man1

uninstall:
	rm -f $(PREFIX)/bin/qstats
	rm -f $(MANPREFIX)/man/man1/qstats.1

.PHONY: all test clean install
