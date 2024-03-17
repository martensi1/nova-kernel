/**
 * cpuid.cpp
 * Implementation of CPUID instruction for CPU identification (only x86)
 * Get vendor name, serial number, version info, features, etc
*/
#include <nova/cpu/cpuid.h>
#include <nova/cpu/flgreg.h>
#include <nova/kernel.h>

using namespace Nova;


enum cpu_id_requests {
    CPUID_LEVEL_AND_VENDOR       = 0x00,
    CPUID_VERSION_AND_FEATURES   = 0x01,
    CPUID_SERIAL_NUMBER          = 0x03,
};


static inline bool has_cpuid(void)
{
    cpu_flag_t flag = CPUFLAG_ID;
    return flagreg_test_if_changeable(flag);
}

static inline void get_cpuid(u32 request, u32* eax, u32* ebx, u32* ecx, u32* edx)
{
    u32 cpuid_eax = request;
    u32 cpuid_ecx = 0;

    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "a" (cpuid_eax), "c" (cpuid_ecx)
    );
}


static void registers_to_string(u32 ebx, u32 ecx, u32 edx, char* result)
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


static void fill_vendor_and_max_input(cpuinfo_x86_t* cpuinfo)
{
    u32 eax, ebx, ecx, edx;
    get_cpuid(CPUID_LEVEL_AND_VENDOR, &eax, &ebx, &ecx, &edx);

    cpuinfo->level = eax;
    registers_to_string(ebx, ecx, edx, cpuinfo->vendor_name);
}

static void fill_version_and_features(cpuinfo_x86_t* cpuinfo)
{
    if (cpuinfo->level <= 0) {
        // Max input value to CPUID is 0
        // No more information can be retrieved
        return;
    }

    u32 eax, ebx, ecx, edx;
    get_cpuid(CPUID_VERSION_AND_FEATURES, &eax, &ebx, &ecx, &edx);

    cpuinfo->stepping = eax & 0xF;
    cpuinfo->model_id = (eax >> 4) & 0xF;
    cpuinfo->family_id = (eax >> 8) & 0xF;
    cpuinfo->processor_type = (eax >> 12) & 0x3;

    // If family ID is 15 (0xF), add extended family ID to family ID
    if (cpuinfo->family_id == 0xF) {
        u8 extended_family_id = (eax >> 20) & 0xFF;
        cpuinfo->family_id = cpuinfo->family_id + extended_family_id;
    }

    // If family ID is 6 (0x6) or 15 (0xF), add extended model ID to model ID
    if (cpuinfo->family_id == 0xF || cpuinfo->family_id == 0x6) {
        u8 extended_model_id = (eax >> 16) & 0xF;
        cpuinfo->model_id = cpuinfo->model_id + (extended_model_id << 4);
    }

    cpuinfo->brand_id = ebx & 0xFF;
    cpuinfo->cache_line_size = (ebx >> 8) & 0xFF;
    cpuinfo->cpu_count = (ebx >> 16) & 0xFF;
    cpuinfo->local_apic_id = (ebx >> 24) & 0xFF;

    cpuinfo->features_ecx = ecx;
    cpuinfo->features_edx = edx;
}


/// @brief Gather CPUID information
/// @param cpuinfo Information structure to fill
void cpuid_identify_cpu(cpuinfo_x86_t& cpuinfo)
{
    if (!has_cpuid()) {
        EnterPanic("CPUID is not supported by the processor");
    }

    Log("Gathering CPUID information...");

    fill_vendor_and_max_input(&cpuinfo);
    fill_version_and_features(&cpuinfo);

    Log("CPUID information successfully obtained");
}

/// @brief Check if the CPU has a specific feature
/// @param features ECX or EDX register value from the CPUID instruction
/// @param feature Feature to check (either CPUID_FEAT_ECX_* or CPUID_FEAT_EDX_*)
/// @return 
bool cpuid_has_feature(const u32 features, u32 feature)
{
    return (features & feature) != 0;
}