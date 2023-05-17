#include <simux/cpuid.h>
#include <simux/atoi.h>
#include <simux/term.h>
#include <stdint.h>


enum cpu_id_requests {
    CPUID_VENDOR_STRING  = 0x00,
    CPUID_FEATURES       = 0x01,
    CPUID_SERIAL_NUMBER  = 0x03,

    // More to come
};

static inline void get_cpuid(uint32_t request, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "a" (request)
    );
}

static void get_cpuid_string(uint32_t request, char* result)
{
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(request, &eax, &ebx, &ecx, &edx);

    result[0] = ebx & 0xFF;
    result[1] = (ebx >> 8) & 0xFF;
    result[2] = (ebx >> 16) & 0xFF;
    result[3] = (ebx >> 24) & 0xFF;

    result[4] = edx & 0xFF;
    result[5] = (edx >> 8) & 0xFF;
    result[6] = (edx >> 16) & 0xFF;
    result[7] = (edx >> 24) & 0xFF;

    result[8] = ecx & 0xFF;
    result[9] = (ecx >> 8) & 0xFF;
    result[10] = (ecx >> 16) & 0xFF;
    result[11] = (ecx >> 24) & 0xFF;

    result[12] = '\0';
}


bool cpuid_has_cpuid(void)
{
    // TODO: Check if cpuid is supported
    return false;
}

void cpuid_get_vendor_name(char* vendor_name)
{
    get_cpuid_string(CPUID_VENDOR_STRING, vendor_name);
}

void cpuid_get_version_info(cpuid_version_info* version_info)
{
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(CPUID_FEATURES, &eax, &ebx, &ecx, &edx);

    version_info->stepping_id = eax & 0xF;
    version_info->model_id = (eax >> 4) & 0xF;
    version_info->family_id = (eax >> 8) & 0xF;
    version_info->processor_type = (eax >> 12) & 0x3;

    if (version_info->family_id == 0xF || version_info->family_id == 0x6) {
        uint8_t extended_model_id = (eax >> 16) & 0xF;
        term_write_str("Hej!\n");
        version_info->model_id = version_info->model_id + (extended_model_id << 4);
    }

    if (version_info->family_id == 0xF) {
        uint8_t extended_family_id = (eax >> 20) & 0xFF;
        version_info->family_id = version_info->family_id + (extended_family_id << 4);
    }

    version_info->brand_id = ebx & 0xFF;
    version_info->cache_line_size = (ebx >> 8) & 0xFF;
    version_info->cpu_count = (ebx >> 16) & 0xFF;
    version_info->local_apic_id = (ebx >> 24) & 0xFF;
}

void cpuid_get_serial_number(char* serial_number)
{
    uint32_t unused, ecx, edx;
    get_cpuid(CPUID_SERIAL_NUMBER, &unused, &unused, &ecx, &edx);

    itoa(edx, serial_number, 16);
    itoa(ecx, serial_number + 4, 16);

    serial_number[8] = '\0';
    // TODO: Serial number parsing not complete
}

bool cpuid_has_features(cpu_id_features features)
{
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(CPUID_FEATURES, &eax, &ebx, &ecx, &edx);

    return (edx & features) == (uint32_t)features;
    // TODO: Seperate logic for ecx and edx
}