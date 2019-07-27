//
// Created by Cheng Ma on 2019-06-30.
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include "util/helper.h" 

struct cmdInfo { 
    // second argument of execv(cmd) should be terminated by NULL pointer char ** cmd; int length; /* length of cmd */
    char** cmd;
    int length;
};

void greeting(); void process(struct cmdInfo *cmd);
void parse_cmd(char* buf, struct cmdInfo *info);
void free_cmd_info(struct cmdInfo* info);

char* working_dir; /* current working dir, used to track the path of bin directory*/

int main() { 
    size_t working_dir_size = 128; 
    working_dir = malloc(working_dir_size*sizeof(char)); 
    getcwd(working_dir, working_dir_size);

    greeting();
    char* pwd = malloc(working_dir_size*sizeof(char));
    while (1) {
        getcwd(pwd, working_dir_size);
        printf("\x1B[36m%s\x1B[0m> ", pwd);
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
        struct cmdInfo *cmd = malloc(sizeof(struct cmdInfo));
        parse_cmd(buffer, cmd);
    
        process(cmd);
        free_cmd_info(cmd);
        free(buffer);
    }
}

void parse_cmd(char* buf, struct cmdInfo *info) {
    int size = 4;
    info->cmd = malloc(size * sizeof(char*));
    int num_of_args = 0;
    int i = 0;
    enum state{SKIP, MATCH};
    enum state status = SKIP;

    int word_start_pos = 0;
    for (i = 0; ; i++) {
        char tmp = buf[i];
        if (status == SKIP) {
            if (tmp == ' ' || tmp == '\n' || tmp == '\t') {
                continue;
            } else {
                word_start_pos = i;
                status = MATCH;
            }
        } else if (status == MATCH) {
            if (tmp == ' ' || tmp == '\n' || tmp == '\t' || tmp == '\0') {
                num_of_args++;
                if (num_of_args > size) {
                    size = size * 3 / 2;
                    info->cmd = realloc(info->cmd, size * sizeof(char*));
                }

                char *word = malloc((i - word_start_pos + 1) * sizeof(char));
                memcpy(word, &buf[word_start_pos], i-word_start_pos);
                word[i - word_start_pos] = '\0';
                info->cmd[num_of_args-1] = word;
                status = SKIP;
            }
        }
        if (tmp == '\0')
            break;
    }

    if (num_of_args == size) {
        size += 1;
        info->cmd = realloc(info->cmd , size);
    }
    // second argument of execv should be terminated by NULL pointer
    info->cmd[num_of_args] = NULL;
    info->length = num_of_args;
}

void free_cmd_info(struct cmdInfo* info) {
    if (info == NULL) {
        return;
    }
    int i;
    for (i = 0; i < info->length; i++) {
        free(info->cmd[i]);
    }
    free(info->cmd);
    free(info);
}

void greeting() {
    // print some greeting words
    printf("############################################################\n");
    printf("#                                                          #\n");
    printf("#                                                          #\n");
    printf("#                 welcome to mc shell!                     #\n");
    printf("#                                                          #\n");
    printf("#                                                          #\n");
    printf("############################################################\n");
    printf("\n\n");
}

void process(struct cmdInfo *cmd) {
    // directly implement cd function because changing dir will not work in child process
    if (strcmp("cd", cmd->cmd[0]) == 0) {
        if (cmd->length >= 1) {
            int rtn = chdir(cmd->cmd[1]);
            if (rtn != 0) {
                printf("error running cd, err msg %d\n", rtn);
            }
        }
        return;
    }

    pid_t pid = fork();
    // TODO: mask sigint and other related signal....
    if (pid < 0) {
        fprintf(stderr, "error when fork()\n");
        return;
    } else if (pid == 0) {
        // child will do cmd specified in buf
        // TODO: use checkAndExec function
        if (strcmp("ls", cmd->cmd[0]) == 0) {
            execv(stringConcat(working_dir, "/bin/ls"), cmd->cmd);
        } else if (strcmp("pwd", cmd->cmd[0]) == 0) {
            execv(stringConcat(working_dir, "/bin/pwd"), cmd->cmd);
        }
    } else {
        // parent will wait until child ends
        // TODO: retrieve child status
        waitpid(pid, NULL, 0);
    }
}
