/**
 * setup.cpp
 * Responsible for setting up the CPU (IDT, GDT, etc.)
*/
#include <nova/cpu/setup.h>
#include <nova/cpu/cpuid.h>
#include <nova/cpu/gdt.h>
#include <nova/cpu/idt.h>
#include <nova/cpu/exceptions.h>
#include <nova/cpu/irq.h>
#include <nova/cpu/sysbus.h>
#include <nova/pic/pic.h>


using namespace nova;


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