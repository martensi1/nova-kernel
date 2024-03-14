/**
 * gdt.h
 * Responisble for setting up the Global Descriptor Table (GDT)
 * The following code is not portable to other architectures
*/
#ifndef __NOVA_GDT_H__
#define __NOVA_GDT_H__

#include <nova/types.h>


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

#define GDT_SEGMENT_CODE_PL0 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(0) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL0 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(0) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_DATA_RDWR

#define GDT_SEGMENT_CODE_PL3 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(3) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL3 GDT_SEGMENT_PRESENT(1)     | GDT_SEGMENT_PRIVILEGE(3) | GDT_SEGMENT_DESCRIPTOR_TYPE(1) | \
                             GDT_SEGMENT_GRANULARITY(1) | GDT_SEGMENT_SIZE(1)      | GDT_SEGMENT_LONG_MODE(0) | GDT_SEGMENT_DATA_RDWR


typedef struct __attribute__((packed)) {
    u16 size;
    u32 offset;
} GDTR;


class GDT
{
public:
    /// @brief Initializes the Global Descriptor Table (GDT)
    /// @param write_address Location in memory where the GDT should be written
    /// @param gdt_size Size of the GDT in bytes (will be set by the function)
    static void setup(const u32 write_address, u16& gdt_size);

    // Define possible segment register values (16 bit)
    // The first 13 bits are the index of the segment in the GDT
    // Last 3 bits are if it's a GDT or LDT and the privilege level
    // 0x08 -> kernel code segment (index 1, GDT, privilege level 0)
    // 0x10 -> kernel data segment (index 2, GDT, privilege level 0)
    enum SegmentRegisterValues : u16 {
        KERNEL_CODE_SEGMENT = 0x08,
        KERNEL_DATA_SEGMENT = 0x10,
    };

private:
    static void writeTable(u32 write_address, GDTR& gdtr);
    static void loadTable(const GDTR& gdtr);

    static void* writeDescriptor(void* dest, u32 base, u32 limit, u16 flags);

};


#endif // __NOVA_GDT_H__