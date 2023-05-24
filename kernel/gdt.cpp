#include <simux/gdt.h>
#include <simux/kernel.h>
#include <libc/string.h>
#include <stdint.h>


#define GDT_LOCATION 0x0800

// NOT NEEDED FOR NOW
//#if BITS_BIG_ENDIAN == 1
//   #pragma reverse_bitfields on
//#endif

// NOTE!
// The following code is based on the following assumptions:
// - Endianness is little endian
// - Bit fields for individual bytes are ordered from left to right (MSB to LSB)
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
    dest8[i++] = ((flags >> 8) & 0xF0) & ((limit >> 16) & 0x0F);
    
    // Write base (bit 24-31)
    dest8[i++] = (base >> 24) & 0xFF;

    dest8 += i;
    return (void*)dest8;
}


void gdt_initialize(void)
{
    #define GDT_LOCATION 0x0800
    void* destination = (void*)GDT_LOCATION;

    uint8_t* dest8 = (uint8_t*)destination;
    int i = 0;

    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;

    dest8[i++] = 0xFF;
    dest8[i++] = 0xFF;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x92;
    dest8[i++] = 0xCF;
    dest8[i++] = 0x00;

    dest8[i++] = 0xFF;
    dest8[i++] = 0xFF;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x00;
    dest8[i++] = 0x9A;
    dest8[i++] = 0xCF;
    dest8[i++] = 0x00;

    destination = (void*)(dest8 + i);

    goto kor;

    // Create a GDT segment descriptor for the null segment
    destination = write_descriptor(destination, 0, 0, 0);

    // Create GDT segment descriptors for the code and data segments
    destination = write_descriptor(destination, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL0);
    //printf("%d\n", destination);
    destination = write_descriptor(destination, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL0);
    //printf("%d\n", destination);
    destination = write_descriptor(destination, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL3);
    //printf("%d\n", destination);
    destination = write_descriptor(destination, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL3);

    printk("GDT initialized at 0x%x\n", GDT_LOCATION);

    // Load the GDT
    kor:
    struct gdtr gdtr;
    gdtr.size = (uint16_t)((uint32_t)destination - GDT_LOCATION);
    gdtr.offset = GDT_LOCATION;

    printk("Size: %d\n", gdtr.size);
    printk("Offset: %x\n", gdtr.offset);
    printk("Destination: %x\n", destination);
    printk("Location: %x\n", GDT_LOCATION);

    asm volatile("cli");
    asm volatile("lgdt %0" : : "m" (gdtr));

    asm volatile("mov $0x10, %ax\n \
                  mov %ax, %ds\n \
                  mov %ax, %es\n \
                  mov %ax, %fs\n \
                  mov %ax, %gs\n \
                  mov %ax, %ss\n \
                  ljmp $0x08, $next\n \
                  next:");
}


