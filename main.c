//
// Created by Cheng Ma on 2019-06-30.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helper.h"

void greeting();
void validate_and_process(char*);
void process(char*);
//char* strip(char*, int);

int main() {
    greeting();
    while (1) {
        printf("> ");
        char *buffer = NULL;
        size_t read_len;
        size_t len;
        read_len = getline(&buffer, &len, stdin);
        if (-1 == read_len) {
            printf("error when reading from stdin\n");
            continue;
        }

        //TODO: is there a better way to parse without changing buf?
        buffer[read_len - 1] = '\0';
        validate_and_process(buffer);

        free(buffer);
    }
}

void greeting() {
    // print some greeting words
    printf("########################\n");
    printf("#                      #\n");
    printf("#                      #\n");
    printf("# welcome to mc shell! #\n");
    printf("#                      #\n");
    printf("#                      #\n");
    printf("########################\n");
    printf("\n\n");
}

// check if command is valid or not
void validate_and_process(char* buf) {
    char* cmd = strip(buf);
    if (*cmd == '\0') {
        return;
    }
    // currently don't support cmd with spaces in them, like ls -l
    if (strcmp(cmd, "ls") == 0) {
        process(cmd);
    } else {
        printf("unknown cmd\n");
    }
}

void process(char* buf) {
    pid_t pid = fork();
    // TODO: mask sigint and other related signal....
    if (pid < 0) {
        fprintf(stderr, "error when fork()");
        return;
    } else if (pid == 0) {
        // child will do cmd specified in buf
        execl("./bin/ls", "ls", (char*)0);
    } else {
        // parent will wait until child ends
        waitpid(pid, NULL, 0);
    }
}
