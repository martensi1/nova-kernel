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
#include "hbit.h"
#include <nova/kernel.h>


#define TEST_OK(description) logk(" [OK]: %s", description)
#define TEST_FAIL(description, data) kpanic(description, data)


// The __attribute__ ((constructor)) attribute causes the function to 
// be treated as a global constructor to be called automatically before 
// execution enters kmain(). We use this to test if the kernel startup
// code is working correctly.
static bool globalConstructorCalled = false;

__attribute__ ((constructor)) void globalConstructorTest() 
{
    globalConstructorCalled = true;
}

// This is the start address of the kernel.
// Defined in the linker script.
extern u32 kernel_start;


namespace Nova
{
    namespace priv
    {
        ////////////////////////////////////////////////////////////
        static void checkIfMultibootLoaded(u32 bootHandoverEax)
        {
            #define MULTIBOOT_SPEC_MAGIC_BYTE 0x2BADB002

            if (bootHandoverEax == MULTIBOOT_SPEC_MAGIC_BYTE)
            {
                TEST_OK("Boot handover from multiboot compliant bootloader");
            }
            else 
            {
                TEST_FAIL("Boot handover not from a multiboot compliant bootloader", bootHandoverEax);
            }
        }

        ////////////////////////////////////////////////////////////
        static void checkCPUMode(void) 
        {
            u32 cr0;
            asm volatile("mov %%cr0, %0" : "=r" (cr0));

            if (cr0 & 0x1) {
                TEST_OK("CPU in expected mode (32-bit protected mode)");
            }
            else {
                TEST_FAIL("CPU not in expected mode (32-bit protected mode)", cr0);
            }
        }

        ////////////////////////////////////////////////////////////
        static void checkStartAddress(void)
        {
            #define EXPECTED_KERNEL_START_ADDRESS 0x100000

            if (&kernel_start == (u32*)EXPECTED_KERNEL_START_ADDRESS)
            {
                TEST_OK("Kernel start address correct");
            }
            else
            {
                TEST_FAIL("Kernel start address not correct", (u32)&kernel_start);
            }
        }

        ////////////////////////////////////////////////////////////
        static void checkGlobalConstructorsCalled(void)
        {
            if (globalConstructorCalled)
            {
                TEST_OK("Global constructor initialization");
            }
            else
            {
                TEST_FAIL("Global constructors not initialized", 0);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void RunHBIT(u32 bootHandoverEax)
    {
        logk("Running HBIT (Handover Buildt-in Test)");

        priv::checkIfMultibootLoaded(bootHandoverEax);
        priv::checkCPUMode();
        priv::checkStartAddress();
        priv::checkGlobalConstructorsCalled();

        logk("HBIT passed!");
    }
}