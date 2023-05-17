#include <simux/term.h>
#include <simux/cpuid.h>
#include <simux/atoi.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif


void print_cpu_info() {
    char vendor_name[CPUID_MAX_STR_LENGTH+1];
    char serial_number[CPUID_MAX_STR_LENGTH+1];
    struct cpuid_version_info version_info;

    cpuid_get_vendor_name(vendor_name);
    cpuid_get_serial_number(serial_number);
    cpuid_get_version_info(&version_info);

    char s_stepping_id[CPUID_MAX_STR_LENGTH+1];
    char s_model_id[CPUID_MAX_STR_LENGTH+1];
    char s_family_id[CPUID_MAX_STR_LENGTH+1];
    char s_processor_type[CPUID_MAX_STR_LENGTH+1];
    char s_brand_id[CPUID_MAX_STR_LENGTH+1];
    char s_cache_line_size[CPUID_MAX_STR_LENGTH+1];
    char s_cpu_count[CPUID_MAX_STR_LENGTH+1];
    char s_local_apic_id[CPUID_MAX_STR_LENGTH+1];

    uint8_t stepping_id = version_info.stepping_id;
    uint32_t model_id = version_info.model_id;
    uint8_t family_id = version_info.family_id;
    uint8_t processor_type = version_info.processor_type;
    uint8_t brand_id = version_info.brand_id;
    uint8_t cache_line_size = version_info.cache_line_size;
    uint8_t cpu_count = version_info.cpu_count;
    uint8_t local_apic_id = version_info.local_apic_id;

    itoa(stepping_id, s_stepping_id, 10);
    itoa(model_id, s_model_id, 10);
    itoa(family_id, s_family_id, 10);
    itoa(processor_type, s_processor_type, 10);
    itoa(brand_id, s_brand_id, 10);
    itoa(cache_line_size, s_cache_line_size, 10);
    itoa(cpu_count, s_cpu_count, 10);
    itoa(local_apic_id, s_local_apic_id, 10);

    term_write_str("Using CPU of type ");
    term_write_str(vendor_name);
    term_write_str(" (Family: ");
    term_write_str(s_family_id);
    term_write_str(", Model: ");
    term_write_str(s_model_id);
    term_write_str(", Stepping: ");
    term_write_str(s_stepping_id);
    term_write_str(" , Processor type: ");
    term_write_str(s_processor_type);
    term_write_str(", Brand ID: ");
    term_write_str(s_brand_id);
    term_write_str(", Cache line size: ");
    term_write_str(s_cache_line_size);
    term_write_str(", CPU count: ");
    term_write_str(s_cpu_count);
    term_write_str(", Local APIC ID: ");
    term_write_str(s_local_apic_id);
    term_write_str(")\n");
}


extern "C" {
    void kmain() {
        term_initialize();
        term_write_str("\n\n\n");
        term_write_str("Using VGA text mode\n");
        term_write_str("Loading Simux kernel...\n");

        print_cpu_info();

        while (true) {
            //term_write(&c, 1);
        }
    }
}