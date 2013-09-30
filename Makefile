
PREFIX=/usr/local/bin

all:
	cd ./src && make
	mv ./src/qstats ./bin/

clean:
	cd ./src && make clean
	rm ./bin/qstats

install:
	cp ./bin/qstats $(PREFIX)

uninstall:
	rm /usr/local/bin/qstats

.PHONY: all test clean install
