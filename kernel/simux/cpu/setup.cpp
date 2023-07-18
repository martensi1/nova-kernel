/**
 * setup.cpp
 * Responsible for setting up the CPU (IDT, GDT, etc.)
*/
#include <simux/cpu/setup.h>
#include <simux/cpu/cpuid.h>
#include <simux/cpu/gdt.h>
#include <simux/cpu/idt.h>
#include <simux/cpu/exceptions.h>
#include <simux/cpu/irq.h>
#include <simux/cpu/sysbus.h>
#include <simux/pic/pic.h>


static inline void setup_descriptor_tables()
{
    #define GDT_LOCATION 0x800

    u16 gdt_size;
    GDT::setup(GDT_LOCATION, gdt_size);
    
    const u32 idt_location = GDT_LOCATION + gdt_size + 1;

    idt_clear_gates();
    irq_initialize();

    exceptions_setup_gates();
    irq_setup_gates();

    idt_initialize(idt_location);
}

static inline void setup_pic(void)
{
    pic_initialize(IDT_INTERRUPTS_START);
    pic_enable_irqs();
}

static inline void identify_cpu(void)
{
    cpuinfo_x86_t cpuinfo;
    cpuid_identify_cpu(cpuinfo);
}


/// @brief Sets up the CPU
void cpu_setup(void)
{
    asm volatile("cli");

    setup_descriptor_tables();
    setup_pic();
    identify_cpu();

    asm volatile("sti");
}