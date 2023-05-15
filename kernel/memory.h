#ifndef __SIMUX_RUNTIME_MEMORY_H__
#define __SIMUX_RUNTIME_MEMORY_H__

#include <stddef.h>


extern "C" {
    void* memcpy(void *dst, const void* src, int n);
    void* memset(void* ptr, int value, size_t num);
}


#endif // __SIMUX_RUNTIME_MEMORY_H__