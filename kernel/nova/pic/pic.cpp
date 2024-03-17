/*
PIC (Programmable Interrupt Controller) is a chip that is used to manage interrupts.
This file contains functions that are used to configure and manage the PIC (specifically the 8259 chip).
*/
#include "pic.h"
#include <nova/cpu/sysbus.h>


#define PRIMARY_PIC_COMMAND_PORT 0x20
#define PRIMARY_PIC_DATA_PORT 0x21

#define SECONDARY_PIC_COMMAND_PORT 0xA0
#define SECONDARY_PIC_DATA_PORT 0xA1


#define ICW1_INIT 0x10 // Initialization
#define ICW1_ICW4 0x01 // If ICW4 is needed (1) or not (0)
#define ICW4_8086 0x01 // Microprocessor mode (8086/8088: 1, MCS-80/85: 0)

#define PIC_COMMAND_EOI 0x20 // End of Interrupt


using namespace Nova;


void pic_initialize(u8 irq_base)
{
    // The PIC initialization sequence is done by sending a sequence of 
    // ICWs (Initialization Command Words) to the PIC.

    // ICW1: Initialize command
    WriteIO(PRIMARY_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
    WriteIO(SECONDARY_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);

    // ICW2: IDT offset
    WriteIO(PRIMARY_PIC_DATA_PORT, irq_base);
    WriteIO(SECONDARY_PIC_DATA_PORT, irq_base + 8);

    // ICW3: Tell the PIC how it is wired to master/slave
    WriteIO(PRIMARY_PIC_DATA_PORT, 0x04);
    WriteIO(SECONDARY_PIC_DATA_PORT, 0x02);

    // ICW4: Tell the PIC to operate in 8086 mode
    WriteIO(PRIMARY_PIC_DATA_PORT, ICW4_8086);
    WriteIO(SECONDARY_PIC_DATA_PORT, ICW4_8086);

    pic_disable_irqs();
}

void pic_enable_irqs(void)
{
    WriteIO(PRIMARY_PIC_DATA_PORT, 0x00);
    WriteIO(SECONDARY_PIC_DATA_PORT, 0x00);
}

void pic_disable_irqs(void)
{
    WriteIO(PRIMARY_PIC_DATA_PORT, 0xFF);
    WriteIO(SECONDARY_PIC_DATA_PORT, 0xFF);
}

void pic_send_eoi(u8 irq)
{
    if (irq >= 8)
    {
        WriteIO(SECONDARY_PIC_COMMAND_PORT, PIC_COMMAND_EOI);
    }

    // Send EOI to primary PIC regardless of whether the IRQ
    // is handled by the secondary PIC or not.
    // This is because the secondary PIC is wired to the primary PIC
    // and the primary PIC needs to know when the secondary PIC has
    // finished handling an IRQ.
    WriteIO(PRIMARY_PIC_COMMAND_PORT, PIC_COMMAND_EOI);
}