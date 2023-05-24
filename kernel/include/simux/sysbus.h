#ifndef __SIMUX_SYSTEM_BUS_H__
#define __SIMUX_SYSTEM_BUS_H__

#include <stdint.h>


/// @brief Writes value to the specified I/O port
/// @param port I/O port
/// @param value Value to write
static inline void sysbus_io_out(const uint16_t port, const uint8_t value)
{
    asm volatile("out %%al, %%dx" : : "a"(value), "d"(port));
}

/// @brief Reads value from the specified I/O port
/// @param port I/O port
/// @return Read value
static inline uint8_t sysbus_io_in(const uint16_t port)
{
    uint8_t value;
    asm volatile("in %%dx, %%al" : "=a"(value) : "d"(port));

    return value;
}

#endif // __SIMUX_SYSTEM_BUS_H__