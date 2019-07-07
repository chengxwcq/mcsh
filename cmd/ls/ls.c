#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "../../util/helper.h"

struct FileInfo {
    char* name;
    mode_t st_mode;
};

void print_reg(char* name, mode_t mode);
void print_dir(char* name, mode_t mode);
void print_symbolic(char* name, mode_t mode);
void print_other(char* name, mode_t mode);
char* mode_format(mode_t mode);
int fileInfoComparator(const void *p, const void *q); 

int main(int argc, char* argv[]) {
    // read from current dir
    bool detailed = false;
    struct stat buf;
    struct FileInfo *file_info;

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

    int info_size = 10;
    int info_index = 0;
    file_info = malloc(info_size * sizeof(struct FileInfo));

    while ((dir = readdir(dp)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || 
            strcmp(dir->d_name, "..") == 0 || 
            startsWith(".", dir->d_name)) {
            continue;
        }
        // store all necessary info
        if (lstat(dir->d_name, &buf) < 0) {
            fprintf(stderr, "failed to lstat %s, exiting..", dir->d_name);
            return -1;
        }

        if (info_index == info_size) {
            // realloc
            info_size = info_size * 3 >> 1;
            file_info = realloc(file_info, info_size);
        }

        struct FileInfo tmp;
        tmp.name = dir->d_name;
        tmp.st_mode = buf.st_mode;
        file_info[info_index] = tmp;
        info_index++;
    }
    closedir(dp);
    qsort(file_info, info_index, sizeof(struct FileInfo), fileInfoComparator);

    int i;
    for (i = 0; i < info_index; i++) {
        struct FileInfo info;
        info = file_info[i];
        if (detailed) {
            // check if file is regular, dir or symbolic link
            if (S_ISREG(info.st_mode)) {
                print_reg(info.name, info.st_mode);
            } else if (S_ISDIR(info.st_mode)) {
                print_dir(info.name, info.st_mode);
            } else if (S_ISLNK(info.st_mode)) {
                print_symbolic(info.name, info.st_mode);
            } else {
                print_other(info.name, info.st_mode);
            }
        } else {
            printf("%s\t", info.name);
        }
    }
    if (!detailed) {
        printf("\n");
    }
    return 0;
}

void print_reg(char* name, mode_t mode) {
    printf("-%s\t%s\n", mode_format(mode), name);
}

void print_dir(char* name, mode_t mode) {
    printf("d%s\t\033[1;34m%s\033[0m\n", mode_format(mode), name);
}

void print_symbolic(char* name, mode_t mode) {
    printf("-%s\t\033[1;32m%s\033[0m\n", mode_format(mode), name);
}

void print_other(char* name, mode_t mode) {
    printf("-%s\t%s\n", mode_format(mode), name);
}

char* mode_format(mode_t mode) {
    char permission[3] = {'r', 'w', 'x'};
    char* result = (char *)malloc(9 * sizeof(char));
    result[9] = '\0';
    int i;
    for (i = 0; i < 9; i++) {
        int bit = mode & 1;
        if (bit == 0) {
            result[8-i] = '-';
        } else {
            result[8-i] = permission[2-i%3];
        }
        mode = mode >> 1;
    }
    return result;
}

int fileInfoComparator(const void *p, const void *q) {
    const struct FileInfo *ps = (const struct FileInfo*) p;
    const struct FileInfo *qs = (const struct FileInfo*) q;

    int ps_len = strlen(ps->name);
    int qs_len = strlen(qs->name);
    int min = ps_len < qs_len ? ps_len : qs_len;

    int i;
    for (i = 0; i < min; i++) {
        char pc = ps->name[i];
        char qc = qs->name[i];
        pc = tolower(pc);
        qc = tolower(qc);
        if (pc > qc) {
            return 1; 
        } else if (pc < qc) {
            return -1;
        }
    }
    return min == qs_len ? 1 : -1;
}

