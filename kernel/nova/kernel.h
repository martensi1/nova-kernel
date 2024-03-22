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
#ifndef NOVA_KERNEL_H
#define NOVA_KERNEL_H

#include <nova/common.h>


namespace nova
{
    ////////////////////////////////////////////////////////////
    /// \brief Enter a panic state and halt the kernel
    ///
    /// \param message The error message to display
    /// \param data    Additional data to display (e.g. error code)
    ///
    ////////////////////////////////////////////////////////////
    void EnterPanic(const char* message, u32 data = 0x00000000);

    ////////////////////////////////////////////////////////////
    /// \brief Halt the kernel
    ///
    ////////////////////////////////////////////////////////////
    void HaltKernel(void);

    ////////////////////////////////////////////////////////////
    /// \brief Log a message to the kernel log (terminal and/or file)
    ///
    /// \param format The format string
    /// \param ...    The arguments to the format string
    ///
    ////////////////////////////////////////////////////////////
    void Log(const char* format, ...);

    ////////////////////////////////////////////////////////////
    /// \brief Print a raw message to the terminal
    ///
    /// \param format The format string
    /// \param ...    The arguments to the format string
    ///
    ////////////////////////////////////////////////////////////
    void Print(const char* format, ...);
}

#endif // NOVA_KERNEL_H