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
#ifndef NOVA_ELF32_H
#define NOVA_ELF32_H

#include <nova/types.h>

#define ELF_IDENT_SIZE          16

#define ELF_FORMAT_32           0x01
#define ELF_FORMAT_64           0x02

#define ELF_ENDIAN_LITTLE       0x01
#define ELF_ENDIAN_BIG          0x02

#define ELF_ABI_SYSTEM_V        0x00
#define ELF_ABI_HP_UX           0x01
#define ELF_ABI_NETBSD          0x02
#define ELF_ABI_LINUX           0x03
#define ELF_ABI_GNU_HURD        0x04
#define ELF_ABI_SOLARIS         0x06
#define ELF_ABI_AIX             0x07
#define ELF_ABI_IRIX            0x08
#define ELF_ABI_FREEBSD         0x09
#define ELF_ABI_TRU64           0x0A
#define ELF_ABI_NOVELL_MODESTO  0x0B
#define ELF_ABI_OPENBSD         0x0C
#define ELF_ABI_OPENVMS         0x0D
#define ELF_ABI_NONSTOP_KERNEL  0x0E
#define ELF_ABI_AROS            0x0F
#define ELF_ABI_FENIX_OS        0x10
#define ELF_ABI_CLOUDABI        0x11
#define ELF_ABI_OPENVOS         0x12

#define ELF_ISA_SPARC           0x02
#define ELF_ISA_X86             0x03
#define ELF_ISA_MIPS            0x08
#define ELF_ISA_POWERPC         0x14
#define ELF_ISA_ARM             0x28
#define ELF_ISA_SUPERH          0x2A
#define ELF_ISA_IA_64           0x32
#define ELF_ISA_X86_64          0x3E
#define ELF_ISA_AARCH64         0xB7
#define ELF_ISA_RISC_V          0xF3


// Data types defined by the
// ELF specification
typedef u16 elf32_half_t;
typedef u32 elf32_addr_t;
typedef u32 elf32_off_t;
typedef u32 elf32_word_t;
typedef s32 elf32_sword_t;


typedef enum elf_ident {
    EI_MAG0         = 0, // 0x7F
    EI_MAG1         = 1, // 'E'
    EI_MAG2         = 2, // 'L'
    EI_MAG3         = 3, // 'F'
    EI_CLASS        = 4, // Architecture (32/64-bit)
    EI_DATA         = 5, // Byte order
    EI_VERSION      = 6, // ELF version
    EI_OSABI        = 7, // OS ABI
    EI_ABIVERSION   = 8, // ABI version
    EI_PAD          = 9  // Padding
} elf_ident_t;


typedef struct elf32_header {
    u8 ident[ELF_IDENT_SIZE];
    elf32_half_t type;
    elf32_half_t machine;
    elf32_word_t version;
    elf32_addr_t entry;
    elf32_off_t ph_offset;
    elf32_off_t sh_offset;
    elf32_word_t flags;
    elf32_half_t header_size;
    elf32_half_t ph_entry_size;
    elf32_half_t ph_entry_count;
    elf32_half_t sh_entry_size;
    elf32_half_t sh_entry_count;
    elf32_half_t sh_str_index;
} elf32_header_t;


class elf32_file
{
public:
    elf32_file(const void* data);

    bool is_valid() const;
    bool is_supported(u8 expected_class, u8 expected_data) const;

    elf32_header_t* get_header() const;

private:
    const void* data_;
};

typedef elf32_file elf32_file_t;


#endif // NOVA_ELF32_H