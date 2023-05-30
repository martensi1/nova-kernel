/**
 * irq.h
 * Responisble for setting up the Interrupt Request (IRQ) handlers
*/
#ifndef __SIMUX_IRQ_H__
#define __SIMUX_IRQ_H__


typedef void* (*irq_handler_t)(void);

// IRQ types
typedef enum {
    IRQ0 = 0x00, // PIT
    IRQ1 = 0x01, // Keyboard
    IRQ2 = 0x02, // Cascade
    IRQ3 = 0x03, // COM2
    IRQ4 = 0x04, // COM1
    IRQ5 = 0x05, // LPT2
    IRQ6 = 0x06, // Floppy
    IRQ7 = 0x07, // LPT1
    IRQ8 = 0x08, // CMOS RTC
} irq_number_t;


void irq_setup_gates(void);
void irq_ack(irq_number_t irq);

void irq_add_handler(irq_number_t irq, irq_handler_t handler);
irq_handler_t irq_remove_handler(irq_number_t irq);

inline void irq_enable_interrupts(void)
{
    asm volatile("sti");
}

inline void irq_disable_interrupts(void)
{
    asm volatile("cli");
}


#endif // __SIMUX_IRQ_H__