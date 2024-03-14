#include <libc/string.h>
#undef strcmp


int strcmp(const char* str1, const char* str2)
{
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}
