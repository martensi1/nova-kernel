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


// Field offsets
#define ELF_MAG0 0x7F        // Magic byte 0
#define ELF_MAG1 'E'         // Magic byte 1
#define ELF_MAG2 'L'         // Magic byte 2
#define ELF_MAG3 'F'         // Magic byte 3

#define EI_MAG0     0        // File identification
#define EI_MAG1     1        // File identification
#define EI_MAG2     2        // File identification
#define EI_MAG3     3        // File identification
#define EI_CLASS    4        // File class
#define EI_DATA     5        // Data encoding
#define EI_VERSION  6        // File version
#define EI_OSABI    7        // OS/ABI identification
#define EI_ABIVER   8        // ABI version
#define EI_PAD      9        // Start of padding bytes
#define EI_NIDENT   16       // Size of ident[]

#define ET_NONE 0            // No file type
#define ET_REL  1            // Relocatable file
#define ET_EXEC 2            // Executable file
#define ET_DYN  3            // Shared object file
#define ET_CORE 4            // Core file


// Field values
#define ELF_MAG0 0x7F        // Magic byte 0
#define ELF_MAG1 'E'         // Magic byte 1
#define ELF_MAG2 'L'         // Magic byte 2
#define ELF_MAG3 'F'         // Magic byte 3

#define ELFCLASSNONE 0       // Invalid class
#define ELFCLASS32   1       // 32-bit objects
#define ELFCLASS64   2       // 64-bit objects

#define ELFDATANONE  0       // Invalid data encoding
#define ELFDATA2LSB  1       // 2's complement, little endian
#define ELFDATA2MSB  2       // 2's complement, big endian

#define EM_NONE    0x00      // No machine
#define EM_M32     0x01      // AT&T WE 32100
#define EM_SPARC   0x02      // SPARC
#define EM_386     0x03      // Intel 80386
#define EM_68K     0x04      // Motorola 68000
#define EM_88K     0x05      // Motorola 88000
#define EM_860     0x07      // Intel 80860
#define EM_MIPS    0x08      // MIPS RS3000
#define EM_POWERPC 0x14      // PowerPC
#define EM_ARM     0x28      // ARM
#define EM_SUPERH  0x2A      // SuperH
#define EM_IA_64   0x32      // IA-64
#define EM_X86_64  0x3E      // AMD x86-64
#define EM_AARCH64 0xB7      // ARM AARCH64
#define EM_RISC_V  0xF3      // RISC-V

#define EV_NONE    0         // Invalid version
#define EV_CURRENT 1         // Current version

#define EA_SYSTEM_V     0x00 // UNIX System V ABI
#define EA_HP_UX        0x01 // HP-UX
#define EA_NETBSD       0x02 // NetBSD
#define EA_LINUX        0x03 // Linux
#define EA_GNU_HURD     0x04 // GNU Hurd
#define EA_SOLARIS      0x06 // Solaris
#define EA_AIX          0x07 // AIX
#define EA_IRIX         0x08 // IRIX
#define EA_FREEBSD      0x09 // FreeBSD
#define EA_TRU64        0x0A // TRU64 UNIX
#define EA_MODESTO      0x0B // Novell Modesto
#define EA_OPENBSD      0x0C // OpenBSD
#define EA_OPENVMS      0x0D // OpenVMS
#define EA_NONSTOP_KERN 0x0E // Tandem NonStop Kernel
#define EA_AROS         0x0F // AROS
#define EA_FENIX_OS     0x10 // Fenix OS
#define EA_CLOUDABI     0x11 // Nuxi CloudABI
#define EA_OPENVOS      0x12 // Stratus Technologies OpenVOS

#define ELF_ISA_SPARC   0x02 // SPARC
#define ELF_ISA_X86     0x03 // Intel 80386
#define ELF_ISA_MIPS    0x08 // MIPS RS3000
#define ELF_ISA_POWERPC 0x14 // PowerPC
#define ELF_ISA_ARM     0x28 // ARM
#define ELF_ISA_SUPERH  0x2A // SuperH
#define ELF_ISA_IA_64   0x32 // IA-64
#define ELF_ISA_X86_64  0x3E // AMD x86-64
#define ELF_ISA_AARCH64 0xB7 // ARM AARCH64
#define ELF_ISA_RISC_V  0xF3 // RISC-V

#define PT_NULL      0       // Unused entry
#define PT_LOAD      1       // Loadable segment
#define PT_DYNAMIC   2       // Dynamic linking information
#define PT_INTERP    3       // Interpreter path
#define PT_NOTE      4       // Auxiliary information
#define PT_SHLIB     5       // Reserved
#define PT_PHDR      6       // Program header table


// Data types defined by the ELF specification
typedef u16 elf32_half_t;
typedef u32 elf32_addr_t;
typedef u32 elf32_off_t;
typedef u32 elf32_word_t;
typedef s32 elf32_sword_t;


typedef struct elf32_header {
    u8 ident[EI_NIDENT];            // ELF identification
    elf32_half_t type;              // Object file type
    elf32_half_t machine;           // Machine type
    elf32_word_t version;           // Object file version
    elf32_addr_t entry;             // Entry point address
    elf32_off_t ph_offset;          // Program header offset (in bytes)
    elf32_off_t sh_offset;          // Section header offset (in bytes)
    elf32_word_t flags;             // Processor-specific flags
    elf32_half_t header_size;       // ELF header size (in bytes)
    elf32_half_t ph_entry_size;     // Size of program header entry
    elf32_half_t ph_entry_count;    // Number of program header entries
    elf32_half_t sh_entry_size;     // Size of section header entry
    elf32_half_t sh_entry_count;    // Number of section header entries
    elf32_half_t sh_str_index;      // Section name string table index
} elf32_header_t;

typedef struct elf32_program_header {
    elf32_word_t type;      // Segment type
    elf32_off_t offset;     // Offset of the segment in the file
    elf32_addr_t vaddr;     // Virtual address of the segment in memory
    elf32_addr_t paddr;     // Physical address of the segment in memory
    elf32_word_t file_size; // Size of the segment in the file
    elf32_word_t mem_size;  // Size of the segment in memory
    elf32_word_t flags;     // Segment flags
    elf32_word_t align;     // Segment alignment
} elf32_program_header_t;

typedef struct elf32_section_header {
    elf32_word_t name;
    elf32_word_t type;
    elf32_word_t flags;
    elf32_addr_t addr;
    elf32_off_t offset;
    elf32_word_t size;
    elf32_word_t link;
    elf32_word_t info;
    elf32_word_t addralign;
    elf32_word_t entsize;
} elf32_section_header_t;

typedef struct elf32_file {
    elf32_header_t* header;
    elf32_program_header_t* segments;
    elf32_section_header_t* sections;
} elf32_file_t;


bool parse_elf32(const void* data, elf32_file_t* result);



#endif // NOVA_ELF32_H