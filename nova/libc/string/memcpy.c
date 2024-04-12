#include <libc/string.h>
#undef memcpy


void* memcpy(void *dst, const void* src, size_t n) {
    char* dest = (char*)dst;
    const char* source = (char*)src;

    while (n--) {
        *dest++ = *source++;
    }

    return dst;
}
