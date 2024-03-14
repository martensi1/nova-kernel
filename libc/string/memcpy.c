#include <libc/string.h>
#undef memcpy


void* memcpy(void *dst, const void* src, int n) {
    char* dest = (char*)dst;
    const char* source = (char*)src;

    while (n--) {
        *dest++ = *source++;
    }

    return dst;
}
