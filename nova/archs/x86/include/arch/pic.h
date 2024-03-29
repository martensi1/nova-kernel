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
#ifndef NOVA_PIC_H
#define NOVA_PIC_H

#include <nova/types.h>

namespace nova
{
    ////////////////////////////////////////////////////////////
    /// \brief PIC (Programmable Interrupt Controller) is a chip 
    ///        that is used to manage interrupts. These methods 
    ///        configure and manage the PIC (specifically the 8259 chip).
    ///
    ////////////////////////////////////////////////////////////
    namespace PIC
    {
        ////////////////////////////////////////////////////////////
        /// \brief Initializes the PIC
        ///
        /// \param irqBase The base IRQ number
        ///
        ////////////////////////////////////////////////////////////
        void Initialize(u8 irqBase);

        ////////////////////////////////////////////////////////////
        /// \brief Enables the IRQs
        ///
        ////////////////////////////////////////////////////////////
        void EnableIRQs(void);

        ////////////////////////////////////////////////////////////
        /// \brief Disables the IRQs
        ///
        ////////////////////////////////////////////////////////////
        void DisableIRQs(void);

        ////////////////////////////////////////////////////////////
        /// \brief Sends an EOI (End Of Interrupt) to the PIC
        ///
        /// \param irq The IRQ number
        ///
        ////////////////////////////////////////////////////////////
        void SendEOI(u8 irq);
    }
    void PICInitialize(u8 irq_base);
    void EnableIRQs(void);
    void DisableIRQs(void);
}

#endif // NOVA_PIC_H