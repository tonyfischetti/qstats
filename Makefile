
INSTALLPREFIX=/usr/local
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
	mkdir -p $(INSTALLPREFIX)/bin && cp ./bin/qstats $(INSTALLPREFIX)/bin
	mkdir -p $(MANPREFIX)/man/man1 && cp ./doc/qstats.1 $(MANPREFIX)/man/man1

uninstall:
	test -f $(INSTALLPREFIX)/bin/qstats && rm -f $(INSTALLPREFIX)/bin/qstats
	test -f $(MANPREFIX)/man/man1/qstats.1 && rm -f $(MANPREFIX)/man/man1/qstats.1

.PHONY: all test clean install
