/**
 * idt.h
 * Responisble for setting up the Interrupt Descriptor Table (IDT)
 * The following code is not portable to other architectures
*/
#ifndef __SIMUX_IDT_H__
#define __SIMUX_IDT_H__

#include <simux/types.h>


#define IDT_NUM_ENTRIES 256
#define IDT_ENTRY_SIZE_BYTES 8


// Gate types
#define IDT_GATE_TYPE_TASK             ((0x05) << 0) // Task gate
#define IDT_GATE_TYPE_16BIT_INTERRUPT  ((0x06) << 0) // Interrupt gate (16-bit)
#define IDT_GATE_TYPE_16BIT_TRAP       ((0x07) << 0) // Trap gate (16-bit)
#define IDT_GATE_TYPE_32BIT_INTERRUPT  ((0x0E) << 0) // Interrupt gate (32-bit)
#define IDT_GATE_TYPE_32BIT_TRAP       ((0x0F) << 0) // Trap gate (32-bit)

#define IDT_GATE_PRIVILEGE(x)          (((x) & 0x03) << 6) // Descriptor privilege level (0 - 3)
#define IDT_GATE_PRESENT(x)            (((x) & 0x01) << 7) // Present

#define IDT_INTERRUPT_GATE_PL0 IDT_GATE_PRESENT(1) | IDT_GATE_PRIVILEGE(0) | IDT_GATE_TYPE_32BIT_INTERRUPT
#define IDT_TRAP_GATE_PL0      IDT_GATE_PRESENT(1) | IDT_GATE_PRIVILEGE(0) | IDT_GATE_TYPE_32BIT_TRAP


void idt_clear_gates(void);
void idt_setup_gate(const u8 index, const u32 offset, const u8 gate_type);
void idt_initialize(const u32 location);


#endif // __SIMUX_IDT_H__