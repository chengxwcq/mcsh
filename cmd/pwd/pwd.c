#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

// TODO: starting from current place and traverse upwards to /
int main() {
    size_t working_dir_size = 128;
    char *working_dir;
    working_dir = malloc(working_dir_size*sizeof(char));
    getcwd(working_dir, working_dir_size);
    printf("%s\n", working_dir);
    return 0;
}

