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
#include <arch/pit.h>
#include <arch/sysbus.h>
#include <nova/types.h>
#include <nova/panic.h>
#include <nova/print.h>
#include <nova/sync/spin_guard.h>


static spin_lock lock = spin_lock();


// 1.193182 MHz
#define PIT_TIMER_FREQUENCY 1193182

// The data ports are used to write reload values to the timer
// or to read the current count value
#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_CHANNEL1_DATA_PORT 0x41
#define PIT_CHANNEL2_DATA_PORT 0x42

// The command port is used to send commands to the timer
// For example, to set channel operating mode, access mode, etc.
#define PIT_COMMAND_PORT 0x43

// Depending on operating mode, input ports are used to control if 
// the timer input signal (1.193182 MHz) is sent to channel or not
// NOTE: Only channel 2 has an input port
#define PIT_CHANNEL2_INPUT_PORT 0x61


// The following constants are used to configure the PIT
// The PIT has 3 channels, each channel can be configured separately
// Channel 0: connected to IRQ0
// Channel 1: connected to DRAM refresh (not used)
// Channel 2: connected to PC speaker
#define PIT_SELECT_CHANNEL0 0x00 // Configure channel 0
#define PIT_SELECT_CHANNEL1 0x40 // Configure channel 1
#define PIT_SELECT_CHANNEL2 0x80 // Configure channel 2
#define PIT_COMMAND_READBACK 0xC0 // Read-back command (8254 only)

#define PIT_ACCESS_MODE_LATCH  0x00 // Access mode: latch count value only
#define PIT_ACCESS_MODE_LOBYTE 0x10 // Access mode: read/write low byte only
#define PIT_ACCESS_MODE_HIBYTE 0x20 // Access mode: read/write high byte only
#define PIT_ACCESS_MODE_BOTH   0x30 // Access mode: read/write low byte first, then high byte

#define PIT_OPERATING_MODE_0 0x00 // Interrupt on terminal count
#define PIT_OPERATING_MODE_1 0x02 // Hardware re-triggerable one-shot
#define PIT_OPERATING_MODE_2 0x04 // Rate generator
#define PIT_OPERATING_MODE_3 0x06 // Square wave generator
#define PIT_OPERATING_MODE_4 0x08 // Software triggered strobe
#define PIT_OPERATING_MODE_5 0x0A // Hardware triggered strobe

#define PIT_RELOAD_VALUE_MODE 0x00 // 0 for binary (max 65535), 1 for BCD (max 9999)
#define PIT_RELOAD_VALUE_MAX 65535 // Max value (we always use binary counter mode)


namespace nova
{
    namespace priv
    {
        static void configureChannel(u8 channel, u8 accessMode, u8 operatingMode)
        {
            u8 command = 0;
            command |= channel;
            command |= accessMode;
            command |= operatingMode;
            command |= PIT_RELOAD_VALUE_MODE;

            WriteIO(PIT_COMMAND_PORT, command);
        }

        static void setReloadValue(u8 port, u16 value)
        {
            asm volatile("cli");

            WriteIO(port, value & 0xFF);
            WriteIO(port, (value >> 8) & 0xFF);
        }
    }

    void SetupPIT(const u32 frequency)
    {
        spin_guard guard(lock);

        if (frequency == 0)
        {
            kernel_panic("PIT setup failed: frequency cannot be 0");
        }

        u32 reloadValue = PIT_TIMER_FREQUENCY / frequency;
        u32 actualFrequency = PIT_TIMER_FREQUENCY / reloadValue;

        if (reloadValue > PIT_RELOAD_VALUE_MAX)
        {
            kernel_panic("PIT setup failed: frequency too low");
        }

        priv::configureChannel(
            PIT_SELECT_CHANNEL0, PIT_ACCESS_MODE_BOTH, PIT_OPERATING_MODE_3
        );

        priv::setReloadValue(PIT_CHANNEL0_DATA_PORT, reloadValue);
        log("PIT setup: frequency = %d Hz", actualFrequency);
    }
}