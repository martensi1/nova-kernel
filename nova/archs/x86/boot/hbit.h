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
#ifndef NOVA_HANDOVER_BIT_H
#define NOVA_HANDOVER_BIT_H

#include <nova/types.h>


////////////////////////////////////////////////////////////
/// \brief Run HBIT
///
/// HBIT (Handover Built-in Test) is a test that checks
/// if the kernel was handovered correctly by the bootloader.
/// For example if the bootloader is multiboot compliant, if 
/// the CPU is in 32-bit protected mode, etc.
///
/// boot_handover_eax The value of the EAX register when the 
///                   bootloader handed over control to the 
///                   kernel
///
////////////////////////////////////////////////////////////
void run_hbit(u32 boot_handover_eax);


#endif // NOVA_HANDOVER_BIT_H