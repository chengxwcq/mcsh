all: mcsh ls

mcsh: main.c util/helper.c
	gcc -o mcsh main.c util/helper.c -I.

ls: ./cmd/ls/ls.c util/helper.c
	gcc -o ./bin/ls util/helper.c ./cmd/ls/ls.c
