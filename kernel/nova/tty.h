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
#ifndef NOVA_TERMINAL_H
#define NOVA_TERMINAL_H

#include <nova/common.h>
#include <nova/console/driver.h>
#include <nova/common/cbuffer.h>


namespace Nova
{
    namespace priv
    {
        class TerminalInterface
        {
        public:
            ////////////////////////////////////////////////////////////
            /// \brief Constructs a terminal interface
            ///
            /// \param name The name of the interface
            /// \param driver The driver to use
            ///
            ////////////////////////////////////////////////////////////
            TerminalInterface(const char* name, ConsoleDriver* driver);

            ////////////////////////////////////////////////////////////
            /// \brief Checks if the terminal interface is available
            ///
            /// \return True if the interface is available
            ///
            ////////////////////////////////////////////////////////////
            bool FORCE_INLINE IsAvailable()
            {
                return console_->isAvailable();
            }

            ////////////////////////////////////////////////////////////
            /// \brief Enables the terminal interface
            ///
            ////////////////////////////////////////////////////////////
            void Enable();

            ////////////////////////////////////////////////////////////
            /// \brief Disables the terminal interface
            ///
            ////////////////////////////////////////////////////////////
            void Disable();

            ////////////////////////////////////////////////////////////
            /// \brief Writes data to the terminal
            ///
            /// \param data The data to write
            /// \param size The size of the data
            ///
            ////////////////////////////////////////////////////////////
            void Write(const char* data, size_t size);

            ////////////////////////////////////////////////////////////
            /// \brief Writes a string to the terminal
            ///
            /// \param str The string to write
            ///
            ////////////////////////////////////////////////////////////
            void Write(const char* str);

            ////////////////////////////////////////////////////////////
            /// \brief Flushes the terminal
            ///
            ////////////////////////////////////////////////////////////
            void Flush();

            ////////////////////////////////////////////////////////////
            /// \brief Clears the terminal
            ///
            ////////////////////////////////////////////////////////////
            void Clear();

            ////////////////////////////////////////////////////////////
            /// \brief Checks if the terminal interface is enabled
            ///
            /// \return True if the interface is enabled
            ///
            ////////////////////////////////////////////////////////////
            bool FORCE_INLINE IsEnabled() const
            {
                return enabled_;
            }

            ////////////////////////////////////////////////////////////
            /// \brief Gets the name of the terminal interface
            ///
            /// \return The name of the interface
            ///
            ////////////////////////////////////////////////////////////
            FORCE_INLINE const char* GetName() const
            {
                return name_;
            }

        private:
            FORCE_INLINE void writeData(const char* data, size_t size);
            FORCE_INLINE void writeData(const char* str);

            const char* name_;
            bool enabled_;

            CircularBuffer<256> buffer_;
            SpinLock lock_;

            ConsoleDriver* console_;

        };
    }

    ////////////////////////////////////////////////////////////
    /// \brief Scans for terminal drivers
    ///
    ////////////////////////////////////////////////////////////
    void TerminalScanDrivers();

    ////////////////////////////////////////////////////////////
    /// \brief Initializes the terminal
    ///
    ////////////////////////////////////////////////////////////
    void FORCE_INLINE InitializeTerminal()
    {
        TerminalScanDrivers();
    }

    ////////////////////////////////////////////////////////////
    /// \brief Clears the terminal
    ///
    ////////////////////////////////////////////////////////////
    void TerminalClear();

    ////////////////////////////////////////////////////////////
    /// \brief Writes data to the terminal
    ///
    /// \param data The data to write
    /// \param size The size of the data
    ///
    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* data, size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Writes a string to the terminal
    ///
    /// \param str The string to write
    ///
    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* str);

    ////////////////////////////////////////////////////////////
    /// \brief Flushes the terminal
    ///
    ////////////////////////////////////////////////////////////
    void TerminalFlush();

}

#endif // NOVA_TERMINAL_H