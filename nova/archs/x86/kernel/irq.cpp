/**
 * irq.cpp
 * Responisble for setting up the Interrupt Request (IRQ) handlers
*/
#include <arch/irq.h>
#include <arch/idt.h>
#include <arch/pic.h>
#include <nova/print.h>
#include <nova/panic.h>
#include <nova/sync/spin_guard.h>
#include <libc/string.h>

using namespace nova;


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

static irq_handler_t irq_handlers[IRQ_MAX] = { 0 };
static spin_lock irq_handlers_lock = spin_lock();


static inline void check_bounds(irq_number_t irq)
{
    if (irq < 0 || irq >= IRQ_MAX)
    {
        kernel_panic("IRQ out of range", irq);
    }
}

void irq_initialize(void)
{
    memset(irq_handlers, 0, sizeof(irq_handlers));
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
void irq_add_handler(irq_number_t irq, irq_handler_t handler)
{
    if (handler == NULL) {
        return;
    }

    check_bounds(irq);
    spin_guard guard(irq_handlers_lock);

    irq_handler_t* dest = &irq_handlers[irq];

    if (*dest != NULL) {
        kernel_panic("IRQ handler already set", irq);
    }

    *dest = handler;
}

/// @brief Removes the interrupt handler for the given IRQ
/// @param irq IRQ number
/// @return The removed handler function
irq_handler_t irq_remove_handler(irq_number_t irq)
{
    check_bounds(irq);
    spin_guard guard(irq_handlers_lock);

    irq_handler_t handler = irq_handlers[irq];
    irq_handlers[irq] = NULL;

    return handler;
}

// Called from assembly ISRs
extern "C" void on_irq_interrupt(u32 irq_number, u32 interrupt_index)
{
    static_cast<void>(interrupt_index);

    irq_handlers_lock.aqquire();
    irq_handler_t handler = irq_handlers[irq_number];
    irq_handlers_lock.release();

    if (handler != NULL)
    {
        handler();
    }

    PIC::SendEOI(irq_number);
}