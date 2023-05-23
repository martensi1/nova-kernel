#ifndef __SIMUX_PANIC_H__
#define __SIMUX_PANIC_H__

#include <stdint.h>


void kpanic(const char* message, uint32_t data = 0x00000000);


#endif // __SIMUX_PANIC_H__