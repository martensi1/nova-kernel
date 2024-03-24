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
#include "loader.h"
#include <boot/multiboot.h>

#include <nova/kernel.h>
#include <boot/elf32.h>


namespace nova
{
    static void _are_modules_available(const multiboot_info_t* boot_info)
    {
        multiboot_uint32_t flags = boot_info->flags;

        if (flags & MULTIBOOT_INFO_MODS)
        {
            Log("GRUB modules are available");
        }
        else
        {
            EnterPanic("Failed to locate kernel (GRUB modules not available)");
        }
    }

    static bool _is_module_kernel(const multiboot_module_t* module)
    {
        void* module_start = (void*)module->mod_start;
        void* module_end = (void*)module->mod_end;

        elf32_file_t file(module_start);

        if (!file.is_valid())
        {
            Log("Failed to identify ELF module at 0x%x - 0x%x", module_start, module_end);
            return false;
        }

        elf32_header_t* header = file.get_header();
        
        if (header->ident[EI_CLASS] != ELF_FORMAT_32)
        {
            Log("ELF module is not 32-bit");
            return false;
        }

        return true;
    }

    static const multiboot_module_t* _locate_kernel_module(const multiboot_info_t* boot_info)
    {
        multiboot_module_t* modules = (multiboot_module_t*)boot_info->mods_addr;
        u32 num_modules = boot_info->mods_count;

        for (u32 i = 0; i < num_modules; i++)
        {
            multiboot_module_t* module = &modules[i];

            if (_is_module_kernel(module))
            {
                Log("Kernel module found at 0x%x - 0x%x", module->mod_start, module->mod_end);
                return module;
            }
        }

        return NULL;
    }

    void load_kernel(u32 boot_handover_ebx)
    {
        Log("Reading boot parameters...");

        const multiboot_info_t* boot_info = (multiboot_info_t*)boot_handover_ebx;

        _are_modules_available(boot_info);
        const multiboot_module_t* kernel_module = _locate_kernel_module(boot_info);
        static_cast<void>(kernel_module);
    }
}