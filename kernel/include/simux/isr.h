#ifndef __SIMUX_ISR_H__
#define __SIMUX_ISR_H__

#include <simux/types.h>


void isr_install(const u32 idt_location);


#endif // __SIMUX_ISR_H__