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
#include "loader.h"
#include "multiboot.h"

#include <nova/print.h>
#include <nova/panic.h>
#include <nova/exec/elf32.h>
#include <libc/string.h>


////////////////////////////////////////////////////////////
static bool _are_modules_available(const multiboot_info_t* boot_info)
{
    multiboot_uint32_t flags = boot_info->flags;
    return (flags & MULTIBOOT_INFO_MODS);
}


////////////////////////////////////////////////////////////
static bool _is_module_kernel(const multiboot_module_t* module)
{
    const char* cmdline = (const char*)module->cmdline;
    return (strcmp(cmdline, "nova") == 0);
}


////////////////////////////////////////////////////////////
static void* _load_elf_to_memory(void* elf_start)
{
    elf32_file_t elf;
    if (!parse_elf32(elf_start, &elf))
    {
        return NULL;
    }

    for (u32 i = 0; i < elf.header->sh_entry_count; i++)
    {
        elf32_program_header_t segment = elf.segments[i];

        if (segment.type == PT_LOAD) {
            void* segment_src = (void*)((u8*)elf_start + segment.offset);
            void* segment_dst = (void*)segment.paddr;

            u32 file_size = segment.file_size;
            u32 mem_size = segment.mem_size;

            memcpy(segment_dst, segment_src, file_size);

            if (file_size < mem_size) {
                u32 zero_size = mem_size - file_size;
                memset((u8*)segment_dst + file_size, 0, zero_size);
            }

            log("Loaded segment %d: 0x%x - 0x%x", i, segment_dst, (u8*)segment_dst + mem_size);
        }
    }

    return (void*)elf.header->entry;
}


////////////////////////////////////////////////////////////
static void _jump_to_kernel(void* entrypoint_addr, const multiboot_info_t* boot_info)
{
    log("Jumping to kernel entry point (0x%x)", entrypoint_addr);

    typedef int func(void*);
    func* jump = (func*)entrypoint_addr;

    void* boot_info_addr = (void*)boot_info;
    jump(boot_info_addr);
}


////////////////////////////////////////////////////////////
static void _load_kernel(const multiboot_info_t* boot_info)
{
    multiboot_module_t* modules = (multiboot_module_t*)boot_info->mods_addr;
    u32 num_modules = boot_info->mods_count;

    for (u32 i = 0; i < num_modules; i++)
    {
        multiboot_module_t* module = &modules[i];

        if (!_is_module_kernel(module))
        {
            continue;
        }

        void* module_start = (void*)module->mod_start;
        void* module_end = (void*)module->mod_end;

        log("Kernel ELF found at 0x%x - 0x%x", module->mod_start, module->mod_end);
        void* entry_point = _load_elf_to_memory(module_start);

        if (entry_point == NULL)
        {
            log("Failed to load kernel ELF module at 0x%x - 0x%x", module_start, module_end);
            kernel_panic("Failed to load kernel ELF module");
        }

        _jump_to_kernel(entry_point, boot_info);
    }
}


////////////////////////////////////////////////////////////
void load_kernel(u32 boot_handover_ebx)
{
    log("Reading boot parameters...");
    const multiboot_info_t* params = (multiboot_info_t*)boot_handover_ebx;
    
    if (!_are_modules_available(params))
    {
        kernel_panic("Failed to locate kernel (GRUB modules not available)");
    }

    log("GRUB modules are available");
    _load_kernel(params);
}