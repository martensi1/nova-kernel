/**
 * setup.cpp
 * Responsible for setting up the CPU (IDT, GDT, etc.)
*/
#include <arch/cpu/setup.h>
#include <arch/cpu/cpuid.h>
#include <arch/cpu/gdt.h>
#include <arch/cpu/idt.h>
#include <arch/cpu/exceptions.h>
#include <arch/cpu/irq.h>
#include <arch/cpu/sysbus.h>
#include <arch/pic.h>

using namespace nova;


static inline void setup_descriptor_tables()
{
    idt_clear_gates();
    irq_initialize();

    exceptions_setup_gates();
    irq_setup_gates();

    idt_initialize(0x3000000);
}

static inline void setup_pic(void)
{
    PIC::Initialize(IDT_INTERRUPTS_START);
    PIC::EnableIRQs();
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