#include <simux/term.h>
#include <simux/cpuid.h>
#include <simux/stdio.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif


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
    void kmain() {
        term_initialize();

        cpuinfo_x86 cpuinfo;
        cpuid_identify_cpu(cpuinfo);

        print_cpu_info(&cpuinfo);

        while (true) {
            //term_write(&c, 1);
        }
    }
}