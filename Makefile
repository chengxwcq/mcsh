all: mcsh ls

mcsh: main.c helper.c
	gcc -o mcsh main.c helper.c -I.

ls: ./cmd/ls/ls.c helper.c
	gcc -o ./bin/ls helper.c ./cmd/ls/ls.c
