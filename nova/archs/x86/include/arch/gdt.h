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
#ifndef NOVA_GDT_H
#define NOVA_GDT_H

#include <nova/types.h>


// Define possible segment register values (16 bit)
// The first 13 bits are the index of the segment in the GDT
// Last 3 bits are if it's a GDT or LDT and the privilege level
// 0x08 -> kernel code segment (index 1, GDT, privilege level 0)
// 0x10 -> kernel data segment (index 2, GDT, privilege level 0)
enum segment_register_value : u16 {
    KERNEL_CODE_SEGMENT = 0x08,
    KERNEL_DATA_SEGMENT = 0x10,
};

namespace nova
{
    void setup_gdt(const u32 write_address, u16& gdt_size);
}


#endif // NOVA_GDT_H