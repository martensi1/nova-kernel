#include <libc/string.h>
#undef strcat


char* strcat(char* destination, const char* source)
{
    char* dest = destination;

    while (*dest != '\0') {
        dest++;
    }

    while (*source != '\0') {
        *dest++ = *source++;
    }

    *dest = '\0';
    return destination;
}
