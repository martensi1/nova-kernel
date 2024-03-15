/**
 * gdt.cpp
 * Responisble for setting up the Global Descriptor Table (GDT)
 * The following code is not portable to other architectures
*/
#include <nova/cpu/gdt.h>
#include <nova/kernel.h>
#include <libc/string.h>



void GDT::setup(const u32 writeAddress, u16& gdtSize)
{
    GDTR gdtr;

    writeTable(writeAddress, gdtr);
    loadTable(gdtr);

    gdtSize = gdtr.size;
}

void GDT::writeTable(u32 writeAddress, GDTR& gdtr)
{
    void* dest = (void*)writeAddress;

    // We want to use paging and not segmentation, so we define 
    // 4 large overlapping segments that cover the entire 4GB address space
    dest = writeDescriptor(dest, 0, 0, 0);
    dest = writeDescriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL0);
    dest = writeDescriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL0);
    dest = writeDescriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL3);
    dest = writeDescriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL3);

    gdtr.size = (u16)((u32)dest - writeAddress);
    gdtr.offset = writeAddress;

    logk("Global Descriptor Table (GDT) successfully written to memory");
}

void GDT::loadTable(const GDTR& gdtr)
{
    logk("Loading GDT into processor...");

    // Disable interrupts and update the GDTR register to
    // point to our new GDT
    asm volatile("cli");
    asm volatile("lgdt %0" : : "m" (gdtr));

    // Now when we have a new GDT, we need to set the segment registers
    // to point to the correct segments (for us kernel code and data segments)
    asm volatile("\
        mov %0, %%ax\n \
        mov %%ax, %%ds\n \
        mov %%ax, %%es\n \
        mov %%ax, %%fs\n \
        mov %%ax, %%gs\n \
        mov %%ax, %%ss\n \
        jmp %1, $gdt_jump_%=\n \
        gdt_jump_%=:\n \
        " : : "i" (KERNEL_DATA_SEGMENT), "i" (KERNEL_CODE_SEGMENT));
    

    logk("GDT loaded and activated"); 
}

void* GDT::writeDescriptor(void* dest, u32 base, u32 limit, u16 flags)
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