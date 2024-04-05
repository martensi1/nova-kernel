#include <libc/string.h>
#undef memset


void* memset(void* ptr, int value, size_t num) {
    char* dst = (char*)ptr;

    while (num--) {
        *dst++ = value;
    }

    return ptr;
}
