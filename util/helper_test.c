#include "helper.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main() {
//    char* input[] = {"aa", "orange", "blue", "a"};
//    sortString(input, 4);
//    int i;
//    for (i = 0; i < 4; i++) {
//        printf("%s\n", input[i]);
//    }
    
    char* a = "aaaa";
    char* b = "bbb";
    assert(strcmp(stringConcat(a, b), "aaaabbb") == 0);
}


