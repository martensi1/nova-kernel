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
#ifndef NOVA_PIT_H
#define NOVA_PIT_H

#include <nova/types.h>


namespace nova
{
    ////////////////////////////////////////////////////////////
    /// \brief Sets up the Channel 0 of the PIT to generate 
    ///        interrupts (IRQ0) at the specified frequency
    ///
    /// \param frequency The frequency at which to generate 
    ///                  interrupts (the nearest possible frequency will be used)
    ///
    ////////////////////////////////////////////////////////////
    void SetupPIT(const u32 frequency);
}


#endif // NOVA_PIT_H