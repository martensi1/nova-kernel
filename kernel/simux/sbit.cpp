#include <simux/sbit.h>
#include <simux/kernel.h>


static void check_if_multiboot_loaded(u32 boot_handover_eax)
{
    #define MULTIBOOT_SPEC_MAGIC_BYTE 0x2BADB002

    if (boot_handover_eax == MULTIBOOT_SPEC_MAGIC_BYTE)
    {
        printk("Boot handover from multiboot compliant bootloader\n");
    }
    else 
    {
        kpanic("Boot handover not from a multiboot compliant bootloader", boot_handover_eax);
    }
}

static void check_cpu_mode(void) 
{
    u32 cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));

    if (cr0 & 0x1) {
        printk("CPU in expected mode (32-bit protected mode)\n");
    }
    else {
        kpanic("CPU is not in 32-bit protected mode");
    }
}

static void check_start_address(void)
{
    extern u32 kernel_start;
    #define EXPECTED_KERNEL_START_ADDRESS 0x100000

    if (&kernel_start != (u32*)EXPECTED_KERNEL_START_ADDRESS)
    {
        kpanic("Kernel start address is not 0x100000", (u32)&kernel_start);
    }
}


void sbit_run(u32 boot_handover_eax)
{
    printk("Running SBIT (Start-up Buildt-in Test)\n");

    check_if_multiboot_loaded(boot_handover_eax);
    check_cpu_mode();
    check_start_address();

    printk("SBIT passed!\n");
}