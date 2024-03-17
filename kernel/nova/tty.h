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
#ifndef NOVA_TERMINAL_H
#define NOVA_TERMINAL_H

#include <nova/common.h>


namespace Nova
{
    ////////////////////////////////////////////////////////////
    /// \brief Initializes the terminal
    ///
    ////////////////////////////////////////////////////////////
    void InitializeTerminal();

    ////////////////////////////////////////////////////////////
    /// \brief Clears the terminal
    ///
    ////////////////////////////////////////////////////////////
    void ClearTerminal();

    ////////////////////////////////////////////////////////////
    /// \brief Writes data to the terminal
    ///
    /// \param data The data to write
    /// \param size The size of the data
    ///
    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* data, size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Writes a string to the terminal
    ///
    /// \param str The string to write
    ///
    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* str);

}

#endif // NOVA_TERMINAL_H