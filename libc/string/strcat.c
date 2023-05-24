#include <libc/string.h>
#undef strcat


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
