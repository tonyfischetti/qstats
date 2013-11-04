
INSTALLPREFIX=/usr/local
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
	mkdir -p $(INSTALLPREFIX)/man/man1 && cp ./doc/qstats.1 $(INSTALLPREFIX)/man/man1

uninstall:
	test -f $(INSTALLPREFIX)/bin/qstats && rm -f $(INSTALLPREFIX)/bin/qstats
	test -f $(INSTALLPREFIX)/man/man1/qstats.1 && rm -f $(INSTALLPREFIX)/man/man1/qstats.1

.PHONY: all test clean install
