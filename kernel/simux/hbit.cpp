/**
 * hbit.cpp
 * HBIT (Handover Built-in Test) is a self-test that checks if the kernel was handovered correctly by the bootloader.
 * For example if the bootloader is multiboot compliant, if the CPU is in 32-bit protected mode, etc.
*/
#include <simux/hbit.h>
#include <simux/kernel.h>


#define TEST_OK(description) logk(" [OK]: %s", description)
#define TEST_FAIL(description, data) kpanic(description, data)


static void check_if_multiboot_loaded(u32 boot_handover_eax)
{
    #define MULTIBOOT_SPEC_MAGIC_BYTE 0x2BADB002

    if (boot_handover_eax == MULTIBOOT_SPEC_MAGIC_BYTE)
    {
        TEST_OK("Boot handover from multiboot compliant bootloader");
    }
    else 
    {
        TEST_FAIL("Boot handover not from a multiboot compliant bootloader", boot_handover_eax);
    }
}

static void check_cpu_mode(void) 
{
    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));

    if (cr0 & 0x1) {
        TEST_OK("CPU in expected mode (32-bit protected mode)");
    }
    else {
        TEST_FAIL("CPU not in expected mode (32-bit protected mode)", cr0);
    }
}

static void check_start_address(void)
{
    extern u32 kernel_start;
    #define EXPECTED_KERNEL_START_ADDRESS 0x100000

    if (&kernel_start == (u32*)EXPECTED_KERNEL_START_ADDRESS)
    {
        TEST_OK("Kernel start address correct");
    }
    else
    {
        TEST_FAIL("Kernel start address not correct", (u32)&kernel_start);
    }
}


/// @brief Runs the HBIT (Handover Built-in Test)
/// @param boot_handover_eax The value of the EAX register when the bootloader handed over control to the kernel
void hbit_run(u32 boot_handover_eax)
{
    logk("Running HBIT (Handover Buildt-in Test)");

    check_if_multiboot_loaded(boot_handover_eax);
    check_cpu_mode();
    check_start_address();

    logk("HBIT passed!");
}