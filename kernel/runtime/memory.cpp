#include "memory.h"


extern "C" {
    void* memcpy(void *dst, const void* src, int n) {
        char* dest = reinterpret_cast<char*>(dst);
        const char* source = reinterpret_cast<const char*>(src);

        while (n--) {
            *dest++ = *source++;
        }

        return dst;
    }

    void* memset(void* ptr, int value, size_t num) {
        char* dst = reinterpret_cast<char*>(ptr);

        while (num--) {
            *dst++ = value;
        }

        return ptr;
    }
}