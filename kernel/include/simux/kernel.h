#ifndef __SIMUX_KERNEL_KERNEL_H__
#define __SIMUX_KERNEL_KERNEL_H__

#include <simux/types.h>


void khalt(void);
void kpanic(const char* message, u32 data = 0x00000000);

int printk(const char* format, ...);


#endif // __SIMUX_KERNEL_KERNEL_H__