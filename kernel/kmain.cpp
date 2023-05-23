#include <simux/term.h>
#include <simux/cpuid.h>
#include <simux/stdio.h>
#include <simux/panic.h>
#include <simux/gdt.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
    #error This project must be compiled with a little endian compiler!
#endif


static void check_if_multiboot_loaded(uint32_t boot_handover_eax) {
    #define MULTIBOOT_SPEC_MAGIC_BYTE 0x2BADB002

    if (boot_handover_eax == MULTIBOOT_SPEC_MAGIC_BYTE)
    {
        printf("Boot handover from multiboot compliant bootloader\n");
    }
    else 
    {
        kpanic("Boot handover not from a multiboot compliant bootloader", boot_handover_eax);
    }
}

void check_start_address(void) {
    extern uint32_t kernel_start;
    printf("Kernel start address: 0x%x\n", &kernel_start);
}

void check_cpu_mode(void) {
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));

    if (cr0 & 0x1) {
        printf("CPU in 32-bit protected mode!\n");
    }
    else {
        kpanic("CPU is not in 32-bit protected mode");
    }
}

void print_cpu_info(cpuinfo_x86* cpuinfo) {
    printf("Using CPU of type %s (Family: %d, Model: %d, Stepping: %d, Processor type: %d, Brand ID: %d, Cache line size: %d, CPU count: %d, Local APIC ID: %d)\n",
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


extern "C" {
    void kmain(uint32_t boot_handover_eax) 
    {
        term_initialize();

        check_if_multiboot_loaded(boot_handover_eax);
        check_start_address();
        check_cpu_mode();

        gdt_initialize();

        cpuinfo_x86 cpuinfo;
        cpuid_identify_cpu(cpuinfo);

        print_cpu_info(&cpuinfo);

        while (true) {
            //term_write(&c, 1);
        }
    }
}