#include <libc/stdlib.h>
#undef atoi


int atoi(const char* c)
{
    int n = 0;

    while (*c >= '0' && *c <= '9') {
        n *= 10;
        n += *c - '0';
        c++;
    }
    
    return n;
}