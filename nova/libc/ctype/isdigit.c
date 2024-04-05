#include <libc/stdlib.h>
#undef isdigit


int isdigit(int c)
{
    return c >= '0' && c <= '9';
}