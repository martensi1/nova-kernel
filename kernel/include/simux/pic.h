#ifndef __SIMUX_PIC_H__
#define __SIMUX_PIC_H__

#include <simux/types.h>


void pic_initialize(UInt8 irq_base);

void pic_enable_irqs(void);
void pic_disable_irqs(void);

void pic_send_eoi(UInt8 irq);


#endif // __SIMUX_PIC_H__