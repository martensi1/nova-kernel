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
#include "tty.h"
#include <nova/common.h>
#include <nova/console/driver.h>
#include <libc/string.h>


static ConsoleDriver* choosenDrivers[3] = {NULL, NULL, NULL};
static SpinLock lock = SpinLock();

extern struct ConsoleDriver vgaDriver;
//extern struct ConsoleDriver serialDriver;


namespace Nova
{
    namespace priv
    {
        static void findAndChooseDrivers()
        {
            u32 i = 0;

            if (vgaDriver.isAvailable()) {
                choosenDrivers[i++] = &vgaDriver;
            }

            /*
            if (serialDriver.isAvailable()) {
                choosenDrivers[i++] = &serialDriver;
            }
            */
        }

        static void initializeDriver(ConsoleDriver* driver)
        {
            driver->initialize();
            driver->clear();

            driver->enableCursor();
            driver->updateCursor();

            TerminalWrite("\n\n\n\n\n");
            TerminalWrite("Terminal initialized (");
            TerminalWrite(driver->name);
            TerminalWrite(")\n");
        }

        static void writeStr(ConsoleDriver* driver, const char* data, size_t size)
        {
            for (size_t i = 0; i < size; i++) {
                char c  = data[i];

                if (c == '\n') {
                    driver->writeLineFeed();
                    continue;
                }

                driver->writeChar(c);
            }

            driver->updateCursor();
        }
    }

    void InitializeTerminal()
    {
        priv::findAndChooseDrivers();
    
        for (size_t i = 0; i < 3; i++) {
            if (choosenDrivers[i] != NULL) {
                priv::initializeDriver(choosenDrivers[i]);
            }
        }
    }

    void ClearTerminal()
    {
        for (size_t i = 0; i < 3; i++) {
            if (choosenDrivers[i] != NULL) {
                choosenDrivers[i]->clear();
            }
        }
    }

    void TerminalWrite(const char* data, size_t size)
    {
        SpinGuard guard(lock);

        for (size_t i = 0; i < 3; i++) {
            if (choosenDrivers[i] != NULL) {
                priv::writeStr(choosenDrivers[i], data, size);
            }
        }
    }

    void TerminalWrite(const char* str)
    {
        size_t length = strlen(str);
        TerminalWrite(str, length);
    }
}