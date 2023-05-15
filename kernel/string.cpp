#include "memory.h"


extern "C" {
    size_t strlen(const char* str)
    {
        size_t len = 0;
        while (str[len])
            len++;
        return len;
    }

    int strcmp(const char* str1, const char* str2)
    {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }

        return *str1 - *str2;
    }

    int strncmp(const char* str1, const char* str2, size_t num)
    {
        while (num-- && *str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }

        return *str1 - *str2;
    }

    char* strcpy(char* destination, const char* source)
    {
        char* dest = destination;

        while (*source) {
            *dest++ = *source++;
        }
        *dest = 0;
        
        return destination;   
    }

    char* strncpy(char* destination, const char* source, size_t num)
    {
        char* dest = destination;

        while (num--) {
            if (*source) {
                *dest = *source++;
            }
            else {
                *dest = 0;
            }

            dest++;
        }

        return destination;
    }

    char* strcat(char* destination, const char* source)
    {
        char* dest = destination;

        while (*dest) {
            dest++;
        }

        while (*source) {
            *dest++ = *source++;
        }

        return destination;
    }
}