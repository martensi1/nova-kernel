#ifndef __SIMUX_ISR_H__
#define __SIMUX_ISR_H__

#include <simux/types.h>


enum irq_number {
    IRQ0 = 0x00,
    IRQ1 = 0x01,
    IRQ2 = 0x02,
    IRQ3 = 0x03,
    IRQ4 = 0x04,
    IRQ5 = 0x05,
    IRQ6 = 0x06,
    IRQ7 = 0x07,
    IRQ8 = 0x08,
    IRQ9 = 0x09,
};

typedef void* (*irq_handler)(void);


void isr_install(const u32 idt_location);
void isr_add_interrupt_handler(irq_number irq, irq_handler handler);
void isr_remove_interrupt_handler(irq_number irq);


#endif // __SIMUX_ISR_H__