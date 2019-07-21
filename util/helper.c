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

char* stringConcat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1); size_t len2 = strlen(str2); 
    char* res = malloc((len1+len2+1)*sizeof(char));
    memcpy(res, str1, len1);
    memcpy(res+len1, str2, len2);
    res[len1+len2] = '\0';
    return res;
}


