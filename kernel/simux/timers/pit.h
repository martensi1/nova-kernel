/**
 * pit.h
 * Defines the Programmable Interval Timer (PIT) interface
*/
#ifndef __SIMUX_PIT_H__
#define __SIMUX_PIT_H__

#include <simux/types.h>


void pic_initialize(u8 irq_base);

void pic_enable_irqs(void);
void pic_disable_irqs(void);

void pic_send_eoi(u8 irq);


#endif // __SIMUX_PIT_H__