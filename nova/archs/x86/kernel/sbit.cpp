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
#include <arch/sbit.h>
#include <nova/sync/spin_guard.h>
#include <nova/print.h>
#include <nova/panic.h>
#include <arch/cpu/irq.h>


#define TEST_OK(description) log(" [OK]: %s", description)
#define TEST_FAIL(description, data) kernel_panic(description, data)


// This function is called when IRQ0 is triggered
// It will increase a counter to check if the timer is ticking
static spin_lock timerCounterLock = spin_lock();
static volatile uint8_t timerCounter = 0;

static void onIRQ0()
{
    spin_guard guard(timerCounterLock);
    timerCounter++;
}


////////////////////////////////////////////////////////////
static void _check_pic_timer()
{
    irq_handler_t oldHandler = irq_remove_handler(IRQ0);
    irq_add_handler(IRQ0, (irq_handler_t)onIRQ0);

    bool isTicking = false;

    for (int i = 0; i < 100; i++) {
        asm volatile("hlt");
        spin_guard guard(timerCounterLock);
        
        if (timerCounter > 10) {
            isTicking = true;
            break;
        }
    }

    static_cast<void>(irq_remove_handler(IRQ0));
    static_cast<void>(oldHandler);

    if (isTicking)
    {
        TEST_OK("IRQ0 is ticking");
    }
    else
    {
        TEST_FAIL("IRQ0 not ticking", timerCounter);
    }
}

////////////////////////////////////////////////////////////
void run_sbit()
{
    log("Running SBIT (Startup Buildt-in Test)");

    _check_pic_timer();

    log("SBIT passed!");
}