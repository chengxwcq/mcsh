# has to be util/helper and bin/ls otherwise it will compile these two target everytime `make` gets called

bin_dir = bin
all: mcsh util/helper $(bin_dir)/ls $(bin_dir)/pwd

mcsh: main.c util/helper.c
	gcc -o $@ $^ -I.

$(bin_dir)/ls: ./cmd/ls/ls.c util/helper.c
	gcc -o $@ $^

$(bin_dir)/pwd: ./cmd/pwd/pwd.c
	gcc -o $@ $^

util/helper: util/helper.c util/helper_test.c
	gcc -o $@ $^

.PHONY: clean

clean:
	rm mcsh
	rm $(bin_dir)/*
	rm ./util/helper

