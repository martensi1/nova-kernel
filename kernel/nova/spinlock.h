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
#ifndef NOVA_SPINLOCK_H
#define NOVA_SPINLOCK_H

#include <nova/types.h>


////////////////////////////////////////////////////////////
/// \brief Spinlocks are small and simple locks with minimal overhead. 
///        They are used to protect critical sections of code 
///        from being executed at the same time by multiple threads. 
///        Includes both non-reentrant and reentrant locking (interrupt safe)
///
////////////////////////////////////////////////////////////
class SpinLock
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the spin lock
    ///
    /// \param irqSave
    ///
    ////////////////////////////////////////////////////////////
    SpinLock(bool irqSave=true);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SpinLock();

    ////////////////////////////////////////////////////////////
    /// \brief Aqquires the lock
    ///
    ////////////////////////////////////////////////////////////
    void aqquire();

    ////////////////////////////////////////////////////////////
    /// \brief Releases the lock
    ///
    ////////////////////////////////////////////////////////////
    void release();

private:
    void lock();
    void unlock();

    volatile u32 lock_;
    unsigned long cpuFlags_;
    bool irqSave_;
};


#endif // NOVA_SPINLOCK_H