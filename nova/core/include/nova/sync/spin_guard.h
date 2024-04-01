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
#ifndef NOVA_SPIN_GUARD_H
#define NOVA_SPIN_GUARD_H

#include "spin_lock.h"
#include <nova/types.h>


////////////////////////////////////////////////////////////
/// \brief A scoped lock for spinlocks. Aqquires the specified 
///        lock in the constructor and releases it in the destructor
///
////////////////////////////////////////////////////////////
class spin_guard
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the guard and aqquire the lock
    ///
    /// \param lock The lock to aqquire
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE spin_guard(spin_lock& lock) : lock_(lock)
    {
        lock_.aqquire();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Destructor, releases the spin lock
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE ~spin_guard()
    {
        lock_.release();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    explicit spin_guard(const spin_guard&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted move constructor
    ///
    ////////////////////////////////////////////////////////////
    explicit spin_guard(spin_guard&&) = delete;

private:
    spin_lock& lock_;
};


#endif // NOVA_SPIN_GUARD_H