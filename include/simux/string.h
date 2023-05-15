#ifndef __SIMUX_RUNTIME_MEMORY_H__
#define __SIMUX_RUNTIME_MEMORY_H__

#include <stddef.h>


extern "C" {
    size_t strlen(const char* str);

    int strcmp(const char* str1, const char* str2);
    int strncmp(const char* str1, const char* str2, size_t num);

    char* strcpy(char* destination, const char* source);
    char* strncpy(char* destination, const char* source, size_t num);

    char* strcat(char* destination, const char* source);
}


#endif // __SIMUX_RUNTIME_MEMORY_H__