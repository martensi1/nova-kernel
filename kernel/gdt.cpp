#include <simux/gdt.h>
#include <simux/kernel.h>
#include <libc/string.h>
#include <stdint.h>


// NOTE!
// The following code is based on the following assumptions:
// - Endianness is little endian
// - Bit fields for individual bytes are ordered from left to right (MSB to LSB)
// If not, code will need to be changed accordingly (with #pragma reverse_bitfields on?)
#define SEGMENT_PRESENT(x)          (((x) & 0x01) << 0x07) // Present
#define SEGMENT_PRIVILEGE(x)        (((x) & 0x03) << 0x05) // Privilege level (0 - 3)
#define SEGMENT_DESCRIPTOR_TYPE(x)  (((x) & 0x01) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEGMENT_GRANULARITY(x)      (((x) & 0x01) << 0x0F) // Granularity (limit unit, 0 for bytes, 1 for 4KB pages)
#define SEGMENT_SIZE(x)             (((x) & 0x01) << 0x0E) // Size (0 for 16-bit protected mode segment, 1 for 32-bit protected mode segment)
#define SEGMENT_LONG_MODE(x)        (((x) & 0x01) << 0x0D) // Long mode (1 for 64-bit code segment, 0 otherwise)

#define SEGMENT_DATA_RD             0x00 // Read-Only
#define SEGMENT_DATA_RDEXPD         0x04 // Read-Only, expand-Down
#define SEGMENT_DATA_RDWR           0x02 // Read/Write
#define SEGMENT_DATA_RDWREXPD       0x06 // Read/Write, expand-Down
#define SEGMENT_CODE_EX             0x08 // Execute-Only
#define SEGMENT_CODE_EXC            0x0C // Execute-Only, conforming
#define SEGMENT_CODE_EXRD           0x0A // Execute/Read
#define SEGMENT_CODE_EXRDC          0x0E // Execute/Read, conforming


// Defined segments
#define GDT_SEGMENT_CODE_PL0 SEGMENT_PRESENT(1)     | SEGMENT_PRIVILEGE(0) | SEGMENT_DESCRIPTOR_TYPE(1) | \
                             SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1)      | SEGMENT_LONG_MODE(0) | SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL0 SEGMENT_PRESENT(1)     | SEGMENT_PRIVILEGE(0) | SEGMENT_DESCRIPTOR_TYPE(1) | \
                             SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1)      | SEGMENT_LONG_MODE(0) | SEGMENT_DATA_RDWR

#define GDT_SEGMENT_CODE_PL3 SEGMENT_PRESENT(1)     | SEGMENT_PRIVILEGE(3) | SEGMENT_DESCRIPTOR_TYPE(1) | \
                             SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1)      | SEGMENT_LONG_MODE(0) | SEGMENT_CODE_EXRD

#define GDT_SEGMENT_DATA_PL3 SEGMENT_PRESENT(1)     | SEGMENT_PRIVILEGE(3) | SEGMENT_DESCRIPTOR_TYPE(1) | \
                             SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1)      | SEGMENT_LONG_MODE(0) | SEGMENT_DATA_RDWR


struct gdtr {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));



static void* write_descriptor(void* dest, uint32_t base, uint32_t limit, uint16_t flags)
{
    uint8_t* dest8 = (uint8_t*)dest;
    uint8_t i = 0;

    // Write limit (bit 0-15)
    dest8[i++] = limit & 0xFF;
    dest8[i++] = (limit >> 8) & 0xFF;

    // Write base (bit 0-23)
    dest8[i++] = base & 0xFF;
    dest8[i++] = (base >> 8) & 0xFF;
    dest8[i++] = (base >> 16) & 0xFF;

    // Write pres, priv, desc type, etc
    dest8[i++] = flags & 0xFF;
    dest8[i++] = ((flags >> 8) & 0xF0) | ((limit >> 16) & 0x0F);
    
    // Write base (bit 24-31)
    dest8[i++] = (base >> 24) & 0xFF;

    dest8 += i;
    return (void*)dest8;
}

static void create_gdt_table(struct gdtr* gdtr)
{
    #define GDT_LOCATION 0x0800
    void* dest = (void*)GDT_LOCATION;

    // We want to use paging and not segmentation, so we define 
    // 4 large overlapping segments that cover the entire 4GB address space
    dest = write_descriptor(dest, 0, 0, 0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL3);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL3);

    gdtr->size = (uint16_t)((uint32_t)dest - GDT_LOCATION);
    gdtr->offset = GDT_LOCATION;

    printk("Global Descriptor Table created at 0x%x\n", GDT_LOCATION);
}

static void set_gdtr_register(struct gdtr* gdtr_value)
{
    // Disable interrupts and update the GDTR register to
    // point to our new GDT
    asm volatile("cli");
    asm volatile("lgdt %0" : : "m" (*gdtr_value));

    // Now when we have a new GDT, we need to set the segment registers
    // to point to the correct segments (for us kernel code and data segments)
    //
    // The segment registers are 16 bits, and the first 13 bits are the index
    // of the segment in the GDT, and the last 3 bits are if it's a GDT or LDT
    // and the privilege level
    // 0x08 -> kernel code segment (index 1, GDT, privilege level 0)
    // 0x10 -> kernel data segment (index 2, GDT, privilege level 0)
    asm volatile("\
        mov $0x10, %ax\n \
        mov %ax, %ds\n \
        mov %ax, %es\n \
        mov %ax, %fs\n \
        mov %ax, %gs\n \
        mov %ax, %ss\n \
        jmp $0x08, $gdt_jump\n \
        gdt_jump:\n \
        ");
    
    printk("GDTR register set, reloaded segment descriptors\n");
}


void gdt_initialize(void)
{
    struct gdtr gdtr_value;

    create_gdt_table(&gdtr_value);
    set_gdtr_register(&gdtr_value);
}


