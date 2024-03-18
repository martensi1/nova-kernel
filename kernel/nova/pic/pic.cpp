////////////////////////////////////////////////////////////
//
// Nova OS
// Copyright (C) 2024 Simon Mårtensson
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


namespace Nova
{
    namespace PIC
    {
        void Initialize(u8 irqBase)
        {
            // The PIC initialization sequence is done by sending a sequence of 
            // ICWs (Initialization Command Words) to the PIC.

            // ICW1: Initialize command
            WriteIO(PRIMARY_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
            WriteIO(SECONDARY_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);

            // ICW2: IDT offset
            WriteIO(PRIMARY_PIC_DATA_PORT, irqBase);
            WriteIO(SECONDARY_PIC_DATA_PORT, irqBase + 8);

            // ICW3: Tell the PIC how it is wired to master/slave
            WriteIO(PRIMARY_PIC_DATA_PORT, 0x04);
            WriteIO(SECONDARY_PIC_DATA_PORT, 0x02);

            // ICW4: Tell the PIC to operate in 8086 mode
            WriteIO(PRIMARY_PIC_DATA_PORT, ICW4_8086);
            WriteIO(SECONDARY_PIC_DATA_PORT, ICW4_8086);

            DisableIRQs();
        }

        void EnableIRQs(void)
        {
            WriteIO(PRIMARY_PIC_DATA_PORT, 0x00);
            WriteIO(SECONDARY_PIC_DATA_PORT, 0x00);
        }

        void DisableIRQs(void)
        {
            WriteIO(PRIMARY_PIC_DATA_PORT, 0xFF);
            WriteIO(SECONDARY_PIC_DATA_PORT, 0xFF);
        }

        void SendEOI(u8 irq)
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
    }
}