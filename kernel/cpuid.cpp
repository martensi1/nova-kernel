/**
 * cpuid.cpp
 * Implementation of CPUID instruction for CPU identification (only x86)
 * Get vendor name, serial number, version info, features, etc.
 * For more information about CPUID, see the links below:
 *    https://en.wikipedia.org/wiki/CPUID
 *    https://wiki.osdev.org/CPUID
 *    https://www.felixcloutier.com/x86/cpuid
*/
#include <simux/cpuid.h>
#include <simux/flgreg.h>
#include <simux/stdio.h>
#include <stdint.h>


enum cpu_id_requests {
    CPUID_LEVEL_AND_VENDOR       = 0x00,
    CPUID_VERSION_AND_FEATURES   = 0x01,
    CPUID_SERIAL_NUMBER          = 0x03,
};


static inline bool has_cpuid(void)
{
    cpu_flag flag = CPUFLAG_ID;
    return true;
    return flagreg_test_if_changeable(flag);
}

static inline void get_cpuid(uint32_t request, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    uint32_t cpuid_eax = request;
    uint32_t cpuid_ecx = 0;

    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "a" (cpuid_eax), "c" (cpuid_ecx)
    );
}


static void registers_to_string(uint32_t ebx, uint32_t ecx, uint32_t edx, char* result)
{
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


static void fill_vendor_and_max_input(cpuinfo_x86* cpuinfo)
{
    uint32_t eax, ebx, ecx, edx;
    get_cpuid(CPUID_LEVEL_AND_VENDOR, &eax, &ebx, &ecx, &edx);

    cpuinfo->level = eax;
    registers_to_string(ebx, ecx, edx, cpuinfo->vendor_name);
}

static void fill_version_and_features(cpuinfo_x86* cpuinfo)
{
    if (cpuinfo->level <= 0) {
        // Max input value to CPUID is 0
        // No more information can be retrieved
        return;
    }

    uint32_t eax, ebx, ecx, edx;
    get_cpuid(CPUID_VERSION_AND_FEATURES, &eax, &ebx, &ecx, &edx);

    cpuinfo->stepping = eax & 0xF;
    cpuinfo->model_id = (eax >> 4) & 0xF;
    cpuinfo->family_id = (eax >> 8) & 0xF;
    cpuinfo->processor_type = (eax >> 12) & 0x3;

    // If family ID is 15 (0xF), add extended family ID to family ID
    if (cpuinfo->family_id == 0xF) {
        uint8_t extended_family_id = (eax >> 20) & 0xFF;
        cpuinfo->family_id = cpuinfo->family_id + extended_family_id;
    }

    // If family ID is 6 (0x6) or 15 (0xF), add extended model ID to model ID
    if (cpuinfo->family_id == 0xF || cpuinfo->family_id == 0x6) {
        uint8_t extended_model_id = (eax >> 16) & 0xF;
        cpuinfo->model_id = cpuinfo->model_id + (extended_model_id << 4);
    }

    cpuinfo->brand_id = ebx & 0xFF;
    cpuinfo->cache_line_size = (ebx >> 8) & 0xFF;
    cpuinfo->cpu_count = (ebx >> 16) & 0xFF;
    cpuinfo->local_apic_id = (ebx >> 24) & 0xFF;

    cpuinfo->features_ecx = ecx;
    cpuinfo->features_edx = edx;
}



void cpuid_identify_cpu(cpuinfo_x86& cpuinfo)
{
    if (!has_cpuid()) {
        // Abort if CPUID is not supported
        return;
    }

    fill_vendor_and_max_input(&cpuinfo);
    fill_version_and_features(&cpuinfo);
}

bool cpuid_has_feature(const uint32_t features, uint32_t feature)
{
    return (features & feature) != 0;
}