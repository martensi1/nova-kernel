#include <simux/cpu/isr.h>
#include <simux/cpu/idt.h>
#include <simux/pic/pic.h>
#include <simux/kernel.h>
#include <libc/string.h>

// -- Macros --
#define IRQ_BASE 32
#define IRQ_COUNT IDT_SIZE - IRQ_BASE


// -- Data --
static irq_handler irq_handlers[IRQ_COUNT] = { 0 };

#define LOCATE_EXCEPTION_ISR(x) extern "C" void cpuex_##x(void);
#define ADD_EXCEPTION_ISR(x) idt_set_gate(x, (u32)cpuex_##x, IDT_TRAP_GATE_PL0);

#define LOCATE_IRQ_ISR(x) extern "C" void irq_##x(void);
#define ADD_IRQ_ISR(x) idt_set_gate(x + IRQ_BASE, (u32)irq_##x, IDT_INTERRUPT_GATE_PL0);

LOCATE_EXCEPTION_ISR(0);
LOCATE_EXCEPTION_ISR(1);
LOCATE_EXCEPTION_ISR(2);
LOCATE_EXCEPTION_ISR(3);
LOCATE_EXCEPTION_ISR(4);
LOCATE_EXCEPTION_ISR(5);
LOCATE_EXCEPTION_ISR(6);
LOCATE_EXCEPTION_ISR(7);
LOCATE_EXCEPTION_ISR(8);
LOCATE_EXCEPTION_ISR(9);
LOCATE_EXCEPTION_ISR(10);
LOCATE_EXCEPTION_ISR(11);
LOCATE_EXCEPTION_ISR(12);
LOCATE_EXCEPTION_ISR(13);
LOCATE_EXCEPTION_ISR(14);
LOCATE_EXCEPTION_ISR(15);
LOCATE_EXCEPTION_ISR(16);
LOCATE_EXCEPTION_ISR(17);
LOCATE_EXCEPTION_ISR(18);
LOCATE_EXCEPTION_ISR(19);
LOCATE_EXCEPTION_ISR(20);
LOCATE_EXCEPTION_ISR(21);
LOCATE_EXCEPTION_ISR(22);
LOCATE_EXCEPTION_ISR(23);
LOCATE_EXCEPTION_ISR(24);
LOCATE_EXCEPTION_ISR(25);
LOCATE_EXCEPTION_ISR(26);
LOCATE_EXCEPTION_ISR(27);
LOCATE_EXCEPTION_ISR(28);
LOCATE_EXCEPTION_ISR(29);
LOCATE_EXCEPTION_ISR(30);
LOCATE_EXCEPTION_ISR(31);

LOCATE_IRQ_ISR(0);
LOCATE_IRQ_ISR(1);
LOCATE_IRQ_ISR(2);
LOCATE_IRQ_ISR(3);
LOCATE_IRQ_ISR(4);
LOCATE_IRQ_ISR(5);
LOCATE_IRQ_ISR(6);
LOCATE_IRQ_ISR(7);
LOCATE_IRQ_ISR(8);


// -- Private functions --
static void setup_idt(const u32 idt_location)
{
    idt_clear_gates();

    ADD_EXCEPTION_ISR(0);
    ADD_EXCEPTION_ISR(1);
    ADD_EXCEPTION_ISR(2);
    ADD_EXCEPTION_ISR(3);
    ADD_EXCEPTION_ISR(4);
    ADD_EXCEPTION_ISR(5);
    ADD_EXCEPTION_ISR(6);
    ADD_EXCEPTION_ISR(7);
    ADD_EXCEPTION_ISR(8);
    ADD_EXCEPTION_ISR(9);
    ADD_EXCEPTION_ISR(10);
    ADD_EXCEPTION_ISR(11);
    ADD_EXCEPTION_ISR(12);
    ADD_EXCEPTION_ISR(13);
    ADD_EXCEPTION_ISR(14);
    ADD_EXCEPTION_ISR(15);
    ADD_EXCEPTION_ISR(16);
    ADD_EXCEPTION_ISR(17);
    ADD_EXCEPTION_ISR(18);
    ADD_EXCEPTION_ISR(19);
    ADD_EXCEPTION_ISR(20);
    ADD_EXCEPTION_ISR(21);
    ADD_EXCEPTION_ISR(22);
    ADD_EXCEPTION_ISR(23);
    ADD_EXCEPTION_ISR(24);
    ADD_EXCEPTION_ISR(25);
    ADD_EXCEPTION_ISR(26);
    ADD_EXCEPTION_ISR(27);
    ADD_EXCEPTION_ISR(28);
    ADD_EXCEPTION_ISR(29);
    ADD_EXCEPTION_ISR(30);
    ADD_EXCEPTION_ISR(31);

    ADD_IRQ_ISR(0);
    ADD_IRQ_ISR(1);
    ADD_IRQ_ISR(2);
    ADD_IRQ_ISR(3);
    ADD_IRQ_ISR(4);
    ADD_IRQ_ISR(5);
    ADD_IRQ_ISR(6);
    ADD_IRQ_ISR(7);
    ADD_IRQ_ISR(8);

    idt_write_and_load(idt_location);
}


// -- Public functions --

void isr_install(const u32 idt_location)
{
    setup_idt(idt_location);

    pic_initialize(IRQ_BASE);
    pic_enable_irqs();

    asm volatile("sti");
}


void isr_add_interrupt_handler(irq_number irq, irq_handler handler)
{
    if (irq < 0 || irq >= IRQ_COUNT)
    {
        kpanic("IRQ out of range", irq);
    }

    irq_handler* dest = &irq_handlers[irq];

    if (*dest != NULL) {
        kpanic("IRQ handler already set", irq);
    }

    *dest = handler;
}


irq_handler isr_remove_interrupt_handler(irq_number irq)
{
    if (irq < 0 || irq >= IRQ_COUNT)
    {
        kpanic("IRQ out of range", irq);
    }

    irq_handler handler = irq_handlers[irq];
    irq_handlers[irq] = NULL;

    return handler;
}


// Called by the assembly interrupt handler
extern "C" void on_exception_interrupt(u32 error_code, u32 interrupt_index)
{
    static_cast<void>(error_code);
    kpanic("CPU exception", interrupt_index);
}

extern "C" void on_irq_interrupt(u32 irq_number, u32 interrupt_index)
{
    pic_send_eoi(irq_number);
    static_cast<void>(interrupt_index);

    if (irq_handlers[irq_number] != NULL)
    {
        irq_handlers[irq_number]();
    }
}