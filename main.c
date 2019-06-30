//
// Created by Cheng Ma on 2019-06-30.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void greeting();
void validate_cmd(char*);
void process(char*);

int main() {
    greeting();
    while (1) {
        printf("> ");
        char *buffer = NULL;
        int read_len;
        unsigned int len;
        read_len = getline(&buffer, &len, stdin);
        if (-1 == read_len) {
            printf("error when reading from stdin\n");
            continue;
        }

        // process read
        //TODO: is there a better way to parse without changing buf?
        buffer[read_len - 1] = '\0';
        validate_cmd(buffer);

        free(buffer);
    }
}

void greeting() {
    // print some greeting words
    printf("welcome to mc shell!\n");
}

// check if command is valid or not
void validate_cmd(char* buf) {
    // remove \n from buf
    // currently don't support cmd with spaces in them, like ls -l
    printf("get cmd %s", buf);
    if (strcmp(buf, "ls") == 0) {
//        printf("got ls\n");
        process(buf);
    } else {
        printf("unknown cmd\n");
    }
}

void process(char* buf) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "error when fork()");
        return;
    } else if (pid == 0) {
        // child will do cmd specified in buf
        printf("in process....\n");
        execl("./bin/ls", "ls", (char*)0);
    } else {
        // parent will wait until child ends
        waitpid(pid, NULL, 0);
    }
}
