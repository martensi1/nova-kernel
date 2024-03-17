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
#ifndef NOVA_SYSTEM_BUS_H
#define NOVA_SYSTEM_BUS_H

#include <nova/common.h>


namespace Nova
{
    /// @brief Writes value to the specified I/O port
    /// @param port I/O port
    /// @param value Value to write
    static inline void WriteIO(const u16 port, const u8 value)
    {
        asm volatile("out %%al, %%dx" : : "a"(value), "d"(port));
    }

    /// @brief Reads value from the specified I/O port
    /// @param port I/O port
    /// @return Read value
    static inline u8 ReadIO(const u16 port)
    {
        u8 value;
        asm volatile("in %%dx, %%al" : "=a"(value) : "d"(port));

        return value;
    }
}

#endif // NOVA_SYSTEM_BUS_H