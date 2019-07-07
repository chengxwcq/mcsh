#include "helper.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h> 

char* strip(char* buf) {
    int len = strlen(buf);
    int i = len - 1;
    char *result = buf;
    
    for (; i >= 0; i--) {
        // if is not a space
        if (isspace(buf[i]) == 0) {
            *(buf+i+1) = '\0';
            break;
        }
    }

    for (; *result != '\0'; result++) {
        if (isspace(*result) == 0) {
            break; 
        }
    }
    return result;
}

bool startsWith(const char* pre, const char* str) {
    size_t len_pre = strlen(pre), len_str = strlen(str);
    if (len_pre > len_str)
        return false;
    return strncmp(pre, str, len_pre) == 0;
}

int stringComparator(const void *p, const void *q) {
    char* pchar = *(char **)p;
    char* qchar = *(char **)q;
    return strcmp(pchar, qchar);
}

void sortString(void* base, size_t num) {
    qsort(base, num, sizeof(char *), stringComparator);
}

//int main() {
//    char* input[] = {"aa", "orange", "blue", "a"};
//    sortString(input, 4);
//    int i;
//    for (i = 0; i < 4; i++) {
//        printf("%s\n", input[i]);
//    }
//}

