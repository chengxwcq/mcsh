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

void greeting();
void process(struct cmdInfo *cmd);
struct cmdInfo * parse_cmd(char* buf);
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
        //TODO: is there a better way to parse without changing buf?
        struct cmdInfo *cmd = parse_cmd(buffer);
        process(cmd);
        free_cmd_info(cmd);
        free(buffer);
    }
}

// TODO: use state machine
struct cmdInfo *parse_cmd(char* buf) {
    struct cmdInfo *info = malloc(sizeof(struct cmdInfo));
    int size = 4;
    int num_of_args = 0;
    char* cmd = strip(buf);

    info->cmd = malloc(size * sizeof(char*));
    
    int i;
    int pos_of_arg = 0; /* position of last argument */
    bool in_space = false;
    for (i = 0; ; i++) {
        if (num_of_args == size) {
            size = size * 3 / 2;
            info->cmd = realloc(info->cmd, size * sizeof(char *));
        }
        if (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t' || cmd[i] == '\0') {
            if (in_space && cmd[i] != '\0') {
                pos_of_arg++;
                continue;
            }
            char *tmp = malloc((i - pos_of_arg + 1) * sizeof(char));
            memcpy(tmp, &cmd[pos_of_arg], i-pos_of_arg);
            tmp[i - pos_of_arg] = '\0';
            info->cmd[num_of_args++] = tmp;
            pos_of_arg = i + 1;
            in_space = true;
            if (cmd[i] == '\0') {
                break;
            }
        } else {
            in_space = false;
        }
    }
    info->length = num_of_args;

    if (num_of_args == size) {
        size += 1;
        info->cmd = realloc(info->cmd , size);
    }
    // second argument of execv should be terminated by NULL pointer
    info->cmd[num_of_args] = NULL;
    return info;
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
    printf("########################\n");
    printf("#                      #\n");
    printf("#                      #\n");
    printf("# welcome to mc shell! #\n");
    printf("#                      #\n");
    printf("#                      #\n");
    printf("########################\n");
    printf("\n\n");
}

void process(struct cmdInfo *cmd) {
    // directly implement cd function because changing dir will not work in child process
    if (strcmp("cd", cmd->cmd[0]) == 0) {
        if (cmd->length >= 1) {
            int rtn = chdir(cmd->cmd[1]);
            if (rtn != 0) {
                printf("error running cd, err msg %d", rtn);
            }
        }
        return;
    }

    pid_t pid = fork();
    // TODO: mask sigint and other related signal....
    if (pid < 0) {
        fprintf(stderr, "error when fork()");
        return;
    } else if (pid == 0) {
        // child will do cmd specified in buf
        if (strcmp("ls", cmd->cmd[0]) == 0) {
            execv(stringConcat(working_dir, "/bin/ls"), cmd->cmd);
        } else if (strcmp("pwd", cmd->cmd[0]) == 0) {
            execv("/bin/pwd", cmd->cmd);
        }
    } else {
        // parent will wait until child ends
        waitpid(pid, NULL, 0);
    }
}
