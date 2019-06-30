#ifndef HELPER_FUNCTION_
#define HELPER_FUNCTION_

#include <stdbool.h>

// return string without spaces in the front and end, will return value based on argument, so argument shoule be changable
char* strip(char*);

// check if str starts with pre
bool startsWith(const char* pre, const char* str);

#endif
