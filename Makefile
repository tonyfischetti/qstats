
PREFIX=/usr/local
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

uninstall:
	rm -f $(PREFIX)/bin/qstats

.PHONY: all test clean install
