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


#define ELF_MAG0 0x7F
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'
#define ELF_CURRENT_VERSION 1



elf32_file::elf32_file(const void* data) :
    data_(data)
{

}

bool elf32_file::is_valid() const
{
    elf32_header_t* header = get_header();

    if (!header) {
        return false;
    }

    bool has_magic = 
        header->ident[0] == ELF_MAG0 &&
        header->ident[1] == ELF_MAG1 &&
        header->ident[2] == ELF_MAG2 &&
        header->ident[3] == ELF_MAG3;

    return has_magic;
}



elf32_header_t* elf32_file::get_header() const
{
    return (elf32_header_t*)(data_);
}
