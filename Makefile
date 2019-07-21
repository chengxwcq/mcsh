# has to be util/helper and bin/ls otherwise it will compile these two target everytime `make` gets called
all: mcsh util/helper bin/ls 

mcsh: main.c util/helper.c
	gcc -o $@ $^ -I.

bin/ls: ./cmd/ls/ls.c util/helper.c
	gcc -o $@ $^

util/helper: util/helper.c util/helper_test.c
	gcc -o $@ $^

.PHONY: clean

clean:
	rm mcsh
	rm ./bin/*
	rm ./util/helper

