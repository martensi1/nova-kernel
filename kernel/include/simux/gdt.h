#ifndef __SIMUX_GDT_H__
#define __SIMUX_GDT_H__

#include <simux/types.h>


// Note! The following code is based on the following assumptions:
// - Endianness is little endian
// - Bit fields for individual bytes are ordered from left to right (MSB to LSB)
// - If not, code will need to be changed accordingly (with #pragma reverse_bitfields on?)
#define GDT_SEGMENT_PRESENT(x)          (((x) & 0x01) << 0x07) // Present
#define GDT_SEGMENT_PRIVILEGE(x)        (((x) & 0x03) << 0x05) // Privilege level (0 - 3)
#define GDT_SEGMENT_DESCRIPTOR_TYPE(x)  (((x) & 0x01) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define GDT_SEGMENT_GRANULARITY(x)      (((x) & 0x01) << 0x0F) // Granularity (limit unit, 0 for bytes, 1 for 4KB pages)
#define GDT_SEGMENT_SIZE(x)             (((x) & 0x01) << 0x0E) // Size (0 for 16-bit protected mode segment, 1 for 32-bit protected mode segment)
#define GDT_SEGMENT_LONG_MODE(x)        (((x) & 0x01) << 0x0D) // Long mode (1 for 64-bit code segment, 0 otherwise)

#define GDT_SEGMENT_DATA_RD             0x00 // Read-Only
#define GDT_SEGMENT_DATA_RDEXPD         0x04 // Read-Only, expand-Down
#define GDT_SEGMENT_DATA_RDWR           0x02 // Read/Write
#define GDT_SEGMENT_DATA_RDWREXPD       0x06 // Read/Write, expand-Down
#define GDT_SEGMENT_CODE_EX             0x08 // Execute-Only
#define GDT_SEGMENT_CODE_EXC            0x0C // Execute-Only, conforming
#define GDT_SEGMENT_CODE_EXRD           0x0A // Execute/Read
#define GDT_SEGMENT_CODE_EXRDC          0x0E // Execute/Read, conforming


// Defined segments
#define GDT_SEGMENT_CODE_PL0 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(0) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL0 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(0) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_DATA_RDWR

#define GDT_SEGMENT_CODE_PL3 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(3) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL3 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(3) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_DATA_RDWR

// Define segment register values
// The segment registers are 16 bits, and the first 13 bits are the index
// of the segment in the GDT, and the last 3 bits are if it's a GDT or LDT
// and the privilege level
// 0x08 -> kernel code segment (index 1, GDT, privilege level 0)
// 0x10 -> kernel data segment (index 2, GDT, privilege level 0)
#define CS_KERNEL 0x08
#define DS_KERNEL 0x10


u16 gdt_initialize(const u32 location);


#endif // __SIMUX_GDT_H__