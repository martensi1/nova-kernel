/**
 * hbit.h
 * HBIT (Handover Built-in Test) is a test that checks if the kernel was handovered correctly by the bootloader.
 * For example if the bootloader is multiboot compliant, if the CPU is in 32-bit protected mode, etc.
*/
#ifndef __NOVA_HANDOVER_HBIT_H__
#define __NOVA_HANDOVER_HBIT_H__

#include <nova/types.h>

void hbit_run(u32 boot_handover_eax);


#endif // __NOVA_HANDOVER_HBIT_H__