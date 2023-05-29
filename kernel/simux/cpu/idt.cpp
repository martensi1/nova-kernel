/*
idt.cpp
Responisble for setting up the Interrupt Descriptor Table (IDT)
The following code is not portable to other architectures
*/
#include <simux/cpu/idt.h>
#include <simux/cpu/gdt.h>
#include <simux/kernel.h>
#include <libc/string.h>


struct idt_entry {
    u8 gate_type;
    u32 offset;
} __attribute__((packed));

struct idtr {
    u16 size;
    u32 offset;
} __attribute__((packed));

static idt_entry idt_table[IDT_NUM_ENTRIES];


static void write_descriptor(void* dest, const u32 offset, const u16 segment_selector, const u8 flags)
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
    for (u16 i = 0; i < IDT_NUM_ENTRIES; i++) {
        void* dest = (void*)(location + (i * IDT_ENTRY_SIZE_BYTES));

        // If no gate has been setup for this index, write a null descriptor
        if (idt_table[i].offset == 0) {
            write_descriptor(dest, 0, 0, 0);
        }
        else
        {
            idt_entry* entry = &idt_table[i];
            write_descriptor(dest, entry->offset, CS_KERNEL, entry->gate_type);
        }
    }
    
    idtr_value->size = (IDT_NUM_ENTRIES * IDT_ENTRY_SIZE_BYTES) - 1;
    idtr_value->offset = location;

    logk("Interrupt Descriptor Table (IDT) successfully written to memory\n");
}

static void load_idt_table(struct idtr* idtr_value)
{
    logk("Loading IDT into processor...\n");

    // Disable interrupts and update the IDTR register to
    // point to our new IDT
    asm volatile("cli");
    asm volatile("lidt %0" : : "m" (*idtr_value));
    
    logk("IDT loaded and activated\n");
}


/// @brief Clears all IDT gates (must be called before setting new gates)
void idt_clear_gates(void)
{
    void* dest = (void*)&idt_table;
    size_t size = sizeof(idt_entry) * IDT_ENTRY_SIZE_BYTES;

    memset(dest, 0, size);
}

/// @brief Sets up an IDT gate
/// @param index Index in the IDT table
/// @param offset Interrupt Service Routine (ISR) offset
/// @param gate_type Flags for the IDT gate
void idt_setup_gate(const u8 index, const u32 isr_offset, const u8 gate_type)
{
    idt_table[index].gate_type = gate_type;
    idt_table[index].offset = isr_offset;
}

/// @brief Writes the IDT to memory and loads it into the processor
/// @param location The location in memory to write the IDT to
void idt_initialize(const u32 location)
{
    struct idtr idtr_value;

    write_idt_table(location, &idtr_value);
    load_idt_table(&idtr_value);
}


