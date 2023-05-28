#include <simux/cpu/isr.h>
#include <simux/cpu/idt.h>
#include <simux/pic/pic.h>
#include <simux/kernel.h>
#include <libc/string.h>



extern "C" void on_exception_interrupt(u32 error_code, u32 interrupt_index)
{
    static_cast<void>(error_code);
    kpanic("CPU exception", interrupt_index);
}

extern "C" void on_irq_interrupt(u32 irq_number, u32 interrupt_index)
{
    printk("IRQ handler called! irq: %d, interrupt: %d\n", irq_number, interrupt_index);
    pic_send_eoi(irq_number);
}


// Find 
#define LOCATE_EXCEPTION_ISR(x) extern "C" void cpuex_##x(void);
#define INSTALL_EXCEPTION_ISR(x) idt_set_gate(x, (u32)cpuex_##x, IDT_TRAP_GATE_PL0);

#define LOCATE_IRQ_ISR(x) extern "C" void irq_##x(void);
#define INSTALL_IRQ_ISR(x) idt_set_gate(x + 32, (u32)irq_##x, IDT_INTERRUPT_GATE_PL0);


// CPU exceptions
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

// IRQs
LOCATE_IRQ_ISR(0);
LOCATE_IRQ_ISR(1);
LOCATE_IRQ_ISR(2);
LOCATE_IRQ_ISR(3);
LOCATE_IRQ_ISR(4);
LOCATE_IRQ_ISR(5);
LOCATE_IRQ_ISR(6);
LOCATE_IRQ_ISR(7);
LOCATE_IRQ_ISR(8);



void isr_install(const u32 idt_location)
{
    idt_reset_gates();

    // Add all the exception- and IRQ listeners to the IDT
    INSTALL_EXCEPTION_ISR(0);
    INSTALL_EXCEPTION_ISR(1);
    INSTALL_EXCEPTION_ISR(2);
    INSTALL_EXCEPTION_ISR(3);
    INSTALL_EXCEPTION_ISR(4);
    INSTALL_EXCEPTION_ISR(5);
    INSTALL_EXCEPTION_ISR(6);
    INSTALL_EXCEPTION_ISR(7);
    INSTALL_EXCEPTION_ISR(8);
    INSTALL_EXCEPTION_ISR(9);
    INSTALL_EXCEPTION_ISR(10);
    INSTALL_EXCEPTION_ISR(11);
    INSTALL_EXCEPTION_ISR(12);
    INSTALL_EXCEPTION_ISR(13);
    INSTALL_EXCEPTION_ISR(14);
    INSTALL_EXCEPTION_ISR(15);
    INSTALL_EXCEPTION_ISR(16);
    INSTALL_EXCEPTION_ISR(17);
    INSTALL_EXCEPTION_ISR(18);
    INSTALL_EXCEPTION_ISR(19);
    INSTALL_EXCEPTION_ISR(20);
    INSTALL_EXCEPTION_ISR(21);
    INSTALL_EXCEPTION_ISR(22);
    INSTALL_EXCEPTION_ISR(23);
    INSTALL_EXCEPTION_ISR(24);
    INSTALL_EXCEPTION_ISR(25);
    INSTALL_EXCEPTION_ISR(26);
    INSTALL_EXCEPTION_ISR(27);
    INSTALL_EXCEPTION_ISR(28);
    INSTALL_EXCEPTION_ISR(29);
    INSTALL_EXCEPTION_ISR(30);
    INSTALL_EXCEPTION_ISR(31);

    INSTALL_IRQ_ISR(0);
    INSTALL_IRQ_ISR(1);
    INSTALL_IRQ_ISR(2);
    INSTALL_IRQ_ISR(3);
    INSTALL_IRQ_ISR(4);
    INSTALL_IRQ_ISR(5);
    INSTALL_IRQ_ISR(6);
    INSTALL_IRQ_ISR(7);
    INSTALL_IRQ_ISR(8);

    idt_write_and_load(idt_location);

    pic_initialize(32);
    pic_enable_irqs();

    // asm volatile("sti");
}