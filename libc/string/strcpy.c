#include <libc/string.h>
#undef strcpy


char* strcpy(char* destination, const char* source)
{
    char* dest = destination;

    while (*source) {
        *dest++ = *source++;
    }
    *dest = 0;
    
    return destination;   
}
