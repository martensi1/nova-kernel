/**
 * irq.cpp
 * Responisble for setting up the Interrupt Request (IRQ) handlers
*/
#include "irq.h"
#include <simux/cpu/idt.h>
#include <simux/pic/pic.h>
#include <simux/kernel.h>


#define LOCATE_ISR(x) extern "C" void irq_##x(void);

LOCATE_ISR(0);
LOCATE_ISR(1);
LOCATE_ISR(2);
LOCATE_ISR(3);
LOCATE_ISR(4);
LOCATE_ISR(5);
LOCATE_ISR(6);
LOCATE_ISR(7);
LOCATE_ISR(8);


#define IRQ_MAX IDT_NUM_ENTRIES - IDT_INTERRUPTS_START
static irq_handler irq_handlers[IRQ_MAX] = { 0 };


static inline void check_bounds(irq_number irq)
{
    if (irq < 0 || irq >= IRQ_MAX)
    {
        kpanic("IRQ out of range", irq);
    }
}


/// @brief Sets up the IDT gates for the interrupts (IRQs)
void irq_setup_gates(void)
{
    #define SETUP_GATE(x) idt_setup_gate(IDT_INTERRUPTS_START + x, (u32)irq_##x, IDT_INTERRUPT_GATE_PL0);

    SETUP_GATE(0);
    SETUP_GATE(1);
    SETUP_GATE(2);
    SETUP_GATE(3);
    SETUP_GATE(4);
    SETUP_GATE(5);
    SETUP_GATE(6);
    SETUP_GATE(7);
    SETUP_GATE(8);
}

/// @brief Adds an interrupt handler for the given IRQ
/// @param irq IRQ number
/// @param handler Handler function
void irq_add_handler(irq_number irq, irq_handler handler)
{
    check_bounds(irq);
    irq_handler* dest = &irq_handlers[irq];

    if (*dest != NULL) {
        kpanic("IRQ handler already set", irq);
    }

    *dest = handler;
}

/// @brief Removes the interrupt handler for the given IRQ
/// @param irq IRQ number
/// @return The removed handler function
irq_handler irq_remove_handler(irq_number irq)
{
    check_bounds(irq);

    irq_handler handler = irq_handlers[irq];
    irq_handlers[irq] = NULL;

    return handler;
}


// Called from assembly ISRs
extern "C" void on_irq_interrupt(u32 irq_number, u32 interrupt_index)
{
    pic_send_eoi(irq_number);
    static_cast<void>(interrupt_index);

    logk("IRQ interrupt %d\n", irq_number);

    if (irq_handlers[irq_number] != NULL)
    {
        irq_handlers[irq_number]();
    }
}