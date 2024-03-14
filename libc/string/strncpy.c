#include <libc/string.h>
#undef strncpy


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
