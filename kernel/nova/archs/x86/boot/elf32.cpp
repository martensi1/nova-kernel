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
#include "elf32.h"

namespace nova
{
    namespace priv
    {
        static bool read_magic_number(const u8* elf_data, u32& i)
        {
            return elf_data[i++] == 0x7F && 
                   elf_data[i++] == 'E' &&
                   elf_data[i++] == 'L' &&
                   elf_data[i++] == 'F';
        }

        static bool read_generic_header(const u8* elf_data, u32& i, elf_ident_t& ident)
        {
            if (!read_magic_number(elf_data, i))
                return false;

            ident.format = elf_data[i++];
            ident.endianness = elf_data[i++];
            ident.header_version = elf_data[i++];
            ident.abi = elf_data[i++];

            i += 9; // Skip padding

            i++;
            ident.type = elf_data[i++];
            i++;
            ident.isa = elf_data[i++];
            ident.version = elf_data[i++];

            if (ident.header_version != 1)
                return false;

            return true;
        }
    }

    bool identify_elf(void* elf_data, elf_ident_t& ident)
    {
        const u8* data = static_cast<const u8*>(elf_data);
        u32 i = 0;

        return priv::read_generic_header(data, i, ident);
    }

    bool load_elf(void* elf_data, elf32_header_t& header)
    {
        const u8* data = static_cast<const u8*>(elf_data);
        u32 i = 0;

        static_cast<void>(header);

        if (!priv::read_magic_number(data, i))
            return false;

        return true;
    }
}