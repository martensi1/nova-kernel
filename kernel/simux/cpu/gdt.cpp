/**
 * gdt.cpp
 * Responisble for setting up the Global Descriptor Table (GDT)
 * The following code is not portable to other architectures
*/
#include <simux/cpu/gdt.h>
#include <simux/kernel.h>
#include <libc/string.h>


typedef struct __attribute__((packed)) {
    u16 size;
    u32 offset;
} gdtr_t;



static void* write_descriptor(void* dest, u32 base, u32 limit, u16 flags)
{
    u8* dest8 = (u8*)dest;
    u8 i = 0;

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

static void write_gdt_table(const u32 location, gdtr_t* gdtr_value)
{
    void* dest = (void*)location;

    // We want to use paging and not segmentation, so we define 
    // 4 large overlapping segments that cover the entire 4GB address space
    dest = write_descriptor(dest, 0, 0, 0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL0);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL3);
    dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL3);

    gdtr_value->size = (u16)((u32)dest - location);
    gdtr_value->offset = location;

    logk("Global Descriptor Table (GDT) successfully written to memory\n");
}

static void load_gdt_table(gdtr_t* gdtr_value)
{
    logk("Loading GDT into processor...\n");

    // Disable interrupts and update the GDTR register to
    // point to our new GDT
    asm volatile("cli");
    asm volatile("lgdt %0" : : "m" (*gdtr_value));

    // Now when we have a new GDT, we need to set the segment registers
    // to point to the correct segments (for us kernel code and data segments)
    asm volatile("\
        mov %0, %%ax\n \
        mov %%ax, %%ds\n \
        mov %%ax, %%es\n \
        mov %%ax, %%fs\n \
        mov %%ax, %%gs\n \
        mov %%ax, %%ss\n \
        jmp %1, $gdt_jump\n \
        gdt_jump:\n \
        " : : "i" (DS_KERNEL), "i" (CS_KERNEL));
    

    logk("GDT loaded and activated\n");
}

/// @brief Initializes the Global Descriptor Table (GDT)
/// @param location Location in memory where the GDT should be written
/// @return Size of the GDT
u16 gdt_initialize(const u32 location)
{
    gdtr_t gdtr_value;

    write_gdt_table(location, &gdtr_value);
    load_gdt_table(&gdtr_value);

    return gdtr_value.size;
}


