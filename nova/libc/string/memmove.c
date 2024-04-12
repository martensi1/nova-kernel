#include <libc/string.h>
#undef memmove


void* memmove(void *dst, const void* src, size_t n) {
    char *dest = (char*)dst;
    const char *source = (char*)src;

    if (source < dest) {
        for (dest += n, source += n; n--;) {
            *--dest = *--source;
        }
    }
    else {
        while(n--) {
            *dest++ = *source++;
        }
    }

    return dest;
}