//
// Created by Cheng Ma on 2019-06-30.
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include "util/helper.h"

void greeting();
void process(char**);
char** parse_cmd(char* buf);
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
        // meaning \n
        if (1 == read_len) {
            continue;
        }
        //TODO: is there a better way to parse without changing buf?
        buffer[read_len - 1] = '\0';
        char** cmd = parse_cmd(buffer);
        process(cmd);
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

char** parse_cmd(char* buf) {
    char* cmd = strip(buf);
    int size = 1;
    int index = 0;
    char** res = malloc(size * sizeof(char*));
    
    int i;
    int last_index = 0;
    bool in_space = false;
    for (i = 0; ; i++) {
        if (index == size) {
            size = size * 3 / 2;
            res = realloc(res, size * sizeof(char *));
        }
        if (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t' || cmd[i] == '\0') {
            if (in_space && cmd[i] != '\0') {
                last_index++;
                continue;
            }
            char *tmp = malloc((i - last_index + 1) * sizeof(char));
            memcpy(tmp, &cmd[last_index], i-last_index);
            tmp[i - last_index] = '\0';
            res[index++] = tmp;
            last_index = i + 1;
            in_space = true;
            if (cmd[i] == '\0') {
                break;
            }
        } else {
            in_space = false;
        }
    }

    if (index == size) {
        size += 1;
        res = realloc(res, size);
    }
    res[index] = NULL;

    return res;
}

void process(char** buf) {
    pid_t pid = fork();
    // TODO: mask sigint and other related signal....
    if (pid < 0) {
        fprintf(stderr, "error when fork()");
        return;
    } else if (pid == 0) {
        // child will do cmd specified in buf
        if (strcmp("ls", buf[0]) == 0) {
            execv("./bin/ls", buf);
        }
    } else {
        // parent will wait until child ends
        waitpid(pid, NULL, 0);
    }
}
