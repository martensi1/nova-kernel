/**
 * pit.cpp
 * Defines the Programmable Interval Timer (PIT) interface
*/
#include <nova/timers/pit.h>
#include <nova/cpu/sysbus.h>
#include <nova/spinlock.h>
#include <nova/kernel.h>


static spinlock_t pit_lock = SPINLOCK_UNLOCKED;


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



static void configure_channel(u8 channel, u8 access_mode, u8 operating_mode)
{
    u8 command = 0;
    command |= channel;
    command |= access_mode;
    command |= operating_mode;
    command |= PIT_RELOAD_VALUE_MODE;

    sysbus_io_out(PIT_COMMAND_PORT, command);
}

static void set_reload_value(u8 port, u16 value)
{
    asm volatile("cli");

    sysbus_io_out(port, value & 0xFF);
    sysbus_io_out(port, (value >> 8) & 0xFF);
}


/// @brief Sets up the Channel 0 of the PIT to generate interrupts (IRQ0) at the specified frequency
/// @param frequency The frequency at which to generate interrupts (the nearest possible frequency will be used)
void pic_setup_interrupt_generator(const u32 frequency)
{
    spin_lock_irqsave(pit_lock);

    if (frequency == 0)
    {
        kpanic("PIC setup failed: frequency cannot be 0");
    }

    u32 reload_value = PIT_TIMER_FREQUENCY / frequency;
    u32 actual_frequency = PIT_TIMER_FREQUENCY / reload_value;

    if (reload_value > PIT_RELOAD_VALUE_MAX)
    {
        kpanic("PIC setup failed: frequency too low");
    }

    configure_channel(
        PIT_SELECT_CHANNEL0, PIT_ACCESS_MODE_BOTH, PIT_OPERATING_MODE_3
    );

    set_reload_value(PIT_CHANNEL0_DATA_PORT, reload_value);
    logk("PIC setup: frequency = %d Hz", actual_frequency);

    spin_unlock_irqrestore(pit_lock);
}