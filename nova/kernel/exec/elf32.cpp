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
#include <nova/exec/elf32.h>


bool _is_valid_file(elf32_header_t* header)
{
    if (!header) {
        return false;
    }

    bool has_magic = 
        header->ident[EI_MAG0] == ELF_MAG0 &&
        header->ident[EI_MAG1] == ELF_MAG1 &&
        header->ident[EI_MAG2] == ELF_MAG2 &&
        header->ident[EI_MAG3] == ELF_MAG3;

    return has_magic;   
}


bool parse_elf32(const void* data, elf32_file_t* result)
{
    elf32_header_t* header = (elf32_header_t*)data;

    if (!_is_valid_file(header)) {
        return false;
    }

    if (header->ident[EI_CLASS] != ELFCLASS32) {
        return false;
    }

    if (header->ident[EI_DATA] != ELFDATA2LSB) {
        return false;
    }

    if (header->version != EV_CURRENT) {
        return false;
    }

    if (header->ph_entry_size != sizeof(elf32_program_header_t)) {
        return false;
    }

    if (header->sh_entry_size != sizeof(elf32_section_header_t)) {
        return false;
    }

    result->header = header;
    result->segments = (elf32_program_header_t*)((u8*)data + header->ph_offset);
    result->sections = (elf32_section_header_t*)((u8*)data + header->sh_offset);

    return true;
}
