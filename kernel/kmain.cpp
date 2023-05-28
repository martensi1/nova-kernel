#include <simux/tty.h>
#include <simux/cpu/cpuid.h>
#include <simux/cpu/gdt.h>
#include <simux/cpu/isr.h>
#include <simux/kernel.h>
#include <simux/sbit.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
    #error This project must be compiled with a little endian compiler!
#endif


void print_cpu_info(cpuinfo_x86* cpuinfo)
{
    printk("Using CPU of type %s (Family: %d, Model: %d, Stepping: %d, Processor type: %d, Brand ID: %d, Cache line size: %d, CPU count: %d, Local APIC ID: %d)\n",
        cpuinfo->vendor_name,
        cpuinfo->family_id,
        cpuinfo->model_id,
        cpuinfo->stepping,
        cpuinfo->processor_type,
        cpuinfo->brand_id,
        cpuinfo->cache_line_size,
        cpuinfo->cpu_count,
        cpuinfo->local_apic_id
    );
}

void setup_descriptor_tables()
{
    #define GDT_LOCATION 0x800

    const u16 gdt_size = gdt_initialize(GDT_LOCATION);
    const u32 idt_location = GDT_LOCATION + gdt_size + 1;

    isr_install(idt_location);
}



extern "C" {
    void kmain(u32 boot_handover_eax) 
    {
        term_initialize();
        sbit_run(boot_handover_eax);

        setup_descriptor_tables();

        cpuinfo_x86 cpuinfo;
        cpuid_identify_cpu(cpuinfo);

        print_cpu_info(&cpuinfo);

        while (true) {
            //term_write(&c, 1);
        }
    }
}