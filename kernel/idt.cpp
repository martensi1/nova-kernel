#include <simux/idt.h>
#include <simux/gdt.h>
#include <simux/kernel.h>
#include <libc/string.h>
#include <stdint.h>


struct idtr {
    u16 size;
    u32 offset;
} __attribute__((packed));

struct idt_entry {
    u8 flags;
    u32 offset;
} __attribute__((packed));


#define IDT_SIZE 256
#define IDT_ENTRY_SIZE 8

static idt_entry idt_table[IDT_SIZE];



static void write_descriptor(void* dest, u32 offset, u16 segment_selector, u8 flags)
{
    u8* dest8 = (u8*)dest;
    u8 i = 0;

    // Write offset (bit 0-15)
    dest8[i++] = offset & 0xFF;
    dest8[i++] = (offset >> 8) & 0xFF;

    // Write segment selector
    dest8[i++] = segment_selector & 0xFF;
    dest8[i++] = (segment_selector >> 8) & 0xFF;

    // Write reserved
    dest8[i++] = 0;

    // Write flags
    dest8[i++] = flags;

    // Write offset (bit 16-31)
    dest8[i++] = (offset >> 16) & 0xFF;
    dest8[i++] = (offset >> 24) & 0xFF;
}

static void write_idt_table(const u32 location, struct idtr* idtr_value)
{
    for (u16 i = 0; i < IDT_SIZE; i++) {
        void* dest = (void*)(location + (i * IDT_ENTRY_SIZE));

        // If no function is specified, write a null descriptor
        // Otherwise, write the descriptor
        if (idt_table[i].offset == 0) {
            write_descriptor(dest, 0, 0, 0);
        }
        else
        {
            idt_entry* entry = &idt_table[i];
            write_descriptor(dest, entry->offset, CS_KERNEL, entry->flags);

            /*
            printk("IDT entry %d %x: ", i, entry->offset);
            for (u8 j = 0; j < IDT_ENTRY_SIZE; j++) {
                printk("%02x ", *((u8*)dest + j));
            }
            printk("\n");
            */
        }
    }
    
    idtr_value->size = (IDT_SIZE * IDT_ENTRY_SIZE) - 1;
    idtr_value->offset = location;

    printk("Interrupt Descriptor Table created at 0x%x\n", location);
}

static void set_idtr_register(struct idtr* idtr_value)
{
    // Disable interrupts and update the IDTR register to
    // point to our new IDT
    asm volatile("cli");
    asm volatile("lidt %0" : : "m" (*idtr_value));
    
    printk("IDTR register set\n");
}



void idt_reset_gates(void)
{
    void* dest = (void*)&idt_table;
    memset(dest, 0, sizeof(idt_entry) * IDT_SIZE);
}


void idt_set_gate(const u8 index, const u32 offset, const u16 flags)
{
    idt_table[index].flags = flags;
    idt_table[index].offset = offset;
}


void idt_write_and_load(const u32 location)
{
    struct idtr idtr_value;

    write_idt_table(location, &idtr_value);
    set_idtr_register(&idtr_value);
}


