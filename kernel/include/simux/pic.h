#ifndef __SIMUX_PIC_H__
#define __SIMUX_PIC_H__

#include <simux/types.h>


void pic_initialize(u8 irq_base);

void pic_enable_irqs(void);
void pic_disable_irqs(void);

void pic_send_eoi(u8 irq);


#endif // __SIMUX_PIC_H__