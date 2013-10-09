
PREFIX=/usr/local/bin
BINDIR=./bin/
MKDIR_P=mkdir -p

all: bindir
	cd ./src && make
	mv ./src/qstats ./bin/

bindir:
	$(MKDIR_P) $(BINDIR)

clean:
	cd ./src && make clean
	rm -f ./bin/qstats
	rm -rf ./bin/

install: all
	cp ./bin/qstats $(PREFIX)

uninstall:
	rm -f /usr/local/bin/qstats

.PHONY: all test clean install
