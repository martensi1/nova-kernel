#ifndef __NOVA_KERNEL_KERNEL_H__
#define __NOVA_KERNEL_KERNEL_H__

#include <nova/types.h>


void khalt(void);
void kpanic(const char* message, u32 data = 0x00000000);

void logk(const char* format, ...);
int printk(const char* format, ...);


#endif // __NOVA_KERNEL_KERNEL_H__