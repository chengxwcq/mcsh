#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "../../helper.h"

void print_reg(char* name);
void print_dir(char* name);
void print_symbolic(char* name);
void print_other(char* name);

int main(int argc, char* argv[]) {
    // read from current dir
    bool detailed = false;
    struct stat buf;

    // parse
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            detailed = true;
        } else {
            fprintf(stderr, "not implemented yet");
            return -1;
        }
    }

    DIR *dp;
    struct dirent* dir;
    dp = opendir(".");

    while ((dir = readdir(dp)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || 
            strcmp(dir->d_name, "..") == 0 || 
            startsWith(".", dir->d_name)) {
            continue;
        }
        if (detailed) {
            if (lstat(dir->d_name, &buf) < 0) {
                fprintf(stderr, "failed to lstat %s, exiting..", dir->d_name);
                return -1;
            }
            // check if file is regular, dir or symbolic link
            if (S_ISREG(buf.st_mode)) {
                print_reg(dir->d_name);
            } else if (S_ISDIR(buf.st_mode)) {
                print_dir(dir->d_name);
            } else if (S_ISLNK(buf.st_mode)) {
                print_symbolic(dir->d_name);
            } else {
                print_other(dir->d_name);
            }
        } else {
            printf("%s  ", dir->d_name);
        }
    }
    if (!detailed) {
        printf("\n");
    }

    closedir(dp);
    return 0;
}

void print_reg(char* name) {
    printf("%s\n", name);
}

void print_dir(char* name) {
    printf("%s\n", name);
}

void print_symbolic(char* name) {
    printf("%s\n", name);
}

void print_other(char* name) {
    printf("%s\n", name);
}
