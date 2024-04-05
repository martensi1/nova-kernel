////////////////////////////////////////////////////////////
//
// Nova OS
// Copyright (C) 2024 Simon Alm Mårtensson
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include <arch/gdt.h>
#include <nova/print.h>


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
} gdtr_t;


namespace nova
{
    namespace priv
    {
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

        static void write_table(const u32 write_address, gdtr_t& gdtr)
        {
            void* dest = (void*)write_address;

            // We want to use paging and not segmentation, so we define 
            // 4 large overlapping segments that cover the entire 4GB address space
            dest = write_descriptor(dest, 0, 0, 0);
            dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL0);
            dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL0);
            dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_CODE_PL3);
            dest = write_descriptor(dest, 0, 0xFFFFFFFF, GDT_SEGMENT_DATA_PL3);

            gdtr.size = (u16)((u32)dest - write_address);
            gdtr.offset = write_address;

            log("Global Descriptor Table (GDT) successfully written to memory");
        }

        static void load_table(const gdtr_t& gdtr)
        {
            log("Loading GDT into processor...");

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
            

            log("GDT loaded and activated"); 
        }
    }

    void setup_gdt(const u32 write_address, u16& gdt_size)
    {
        gdtr_t gdtr;

        priv::write_table(write_address, gdtr);
        priv::load_table(gdtr);

        gdt_size = gdtr.size;
    }
}