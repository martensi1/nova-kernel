#ifndef __SIMUX_IDT_H__
#define __SIMUX_IDT_H__

#include <simux/types.h>


// Gate bit flags
#define IDT_GATE_TYPE_TASK             ((0x05) << 7) // Task gate
#define IDT_GATE_TYPE_16BIT_INTERRUPT  ((0x06) << 7) // Interrupt gate (16-bit)
#define IDT_GATE_TYPE_16BIT_TRAP       ((0x07) << 7) // Trap gate (16-bit)
#define IDT_GATE_TYPE_32BIT_INTERRUPT  ((0x0E) << 7) // Interrupt gate (32-bit)
#define IDT_GATE_TYPE_32BIT_TRAP       ((0x0F) << 7) // Trap gate (32-bit)

#define IDT_GATE_PRIVILEGE(x)          (((x) & 0x03) << 3) // Descriptor privilege level (0 - 3)
#define IDT_GATE_PRESENT(x)            (((x) & 0x01) << 0) // Present

// Gate types
//#define IDT_INTERRUPT_GATE_PL0 IDT_GATE_PRESENT(1) | IDT_GATE_PRIVILEGE(0) | IDT_GATE_TYPE_32BIT_INTERRUPT
//#define IDT_TRAP_GATE_PL0      IDT_GATE_PRESENT(1) | IDT_GATE_PRIVILEGE(0) | IDT_GATE_TYPE_32BIT_TRAP
#define IDT_INTERRUPT_GATE_PL0 0x8E
#define IDT_TRAP_GATE_PL0      0x8F


void idt_reset_gates(void);
void idt_set_gate(const u8 index, const u32 offset, const u16 flags);

void idt_write_and_load(const u32 location);


#endif // __SIMUX_IDT_H__