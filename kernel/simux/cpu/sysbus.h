/*
sysbus.h
Functionality for writing to and reading from I/O ports on the system's IO bus
*/
#ifndef __SIMUX_SYSTEM_BUS_H__
#define __SIMUX_SYSTEM_BUS_H__

#include <simux/types.h>


/// @brief Writes value to the specified I/O port
/// @param port I/O port
/// @param value Value to write
static inline void sysbus_io_out(const u16 port, const u8 value)
{
    asm volatile("out %%al, %%dx" : : "a"(value), "d"(port));
}

/// @brief Reads value from the specified I/O port
/// @param port I/O port
/// @return Read value
static inline u8 sysbus_io_in(const u16 port)
{
    u8 value;
    asm volatile("in %%dx, %%al" : "=a"(value) : "d"(port));

    return value;
}

#endif // __SIMUX_SYSTEM_BUS_H__