#include "helper.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

//int main() {
//    char buf[] = "  ba a \n";
//    printf("[%s]\n", buf);
//    printf("[%s]\n", strip(buf));
//}
