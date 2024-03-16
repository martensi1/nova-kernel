#ifndef __NOVA_PIC_H__
#define __NOVA_PIC_H__

#include <nova/common.h>


void pic_initialize(u8 irq_base);

void pic_enable_irqs(void);
void pic_disable_irqs(void);

void pic_send_eoi(u8 irq);


#endif // __NOVA_PIC_H__