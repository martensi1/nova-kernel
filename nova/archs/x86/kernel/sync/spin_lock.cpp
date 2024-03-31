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
#include <nova/sync/spin_lock.h>

#include <arch/flgreg.h>
#include <arch/irq.h>



spin_lock::spin_lock(bool irqSave) :
    lock_(0),
    cpuFlags_(0),
    irqSave_(irqSave)
{
}


void spin_lock::aqquire()
{
    if (irqSave_)
    {
        cpuFlags_ = flagreg_dump();
        irq_disable();
    }

    lock();
}


void spin_lock::release()
{
    unlock();

    if (irqSave_ && flagreg_dump_check_bit(cpuFlags_, CPUFLAG_IF))
    {
        irq_enable();
    }
}