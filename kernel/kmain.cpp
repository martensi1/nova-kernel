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

    char stepping[CPUID_MAX_STR_LENGTH+1];
    char model[CPUID_MAX_STR_LENGTH+1];
    char family[CPUID_MAX_STR_LENGTH+1];

    uint8_t stepping_id = version_info.stepping_id;
    uint16_t model_id = (uint16_t)version_info.extended_model_id << 4 | (uint16_t)version_info.model;
    uint32_t family_id = (uint32_t)version_info.extended_family_id << 4 | (uint32_t)version_info.family_id;

    itoa(stepping_id, stepping, 10);
    itoa(model_id, model, 10);
    itoa(family_id, family, 10);

    term_write_str("Using CPU of type ");
    term_write_str(vendor_name);
    term_write_str(" (Family: ");
    term_write_str(family);
    term_write_str(", Model: ");
    term_write_str(model);
    term_write_str(", Stepping ID: ");
    term_write_str(stepping);
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