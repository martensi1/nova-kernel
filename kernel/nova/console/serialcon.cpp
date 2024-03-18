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
#include <nova/console/driver.h>
#include <nova/cpu/sysbus.h>
#include <nova/serial/serial.h>

using namespace Nova;
static Serial::Port port = Serial::COM1;


static bool IsAvailable(void)
{
    return Serial::isPortAvailable(port);
}

static void Initialize()
{

}

static void WriteChar(const char c)
{
    Serial::writeData(port, c);
}

static void WriteLineFeed(void)
{
    Serial::writeData(port, '\n');
}

static void EnableCursor(void)
{
}

static void UpdateCursor(void)
{
}

static void DisableCursor(void)
{
}

static void Flush(void)
{
    Serial::flush(port);
}

static void Clear(void)
{
    for (int i = 0; i < 80; i++) {
        Serial::writeData(port, '=');
    }

    Serial::writeData(port, '\n');
    Serial::flush(port);
}


// Define driver
struct ConsoleDriver serialDriver = {
    "serialcon",          // name
    IsAvailable,          // isAvailable
    Initialize,           // initialize
    WriteChar,            // writeChar
    WriteLineFeed,        // writeLineFeed
    EnableCursor,         // enableCursor
    UpdateCursor,         // updateCursor
    DisableCursor,        // disableCursor
    Flush,                // flush
    Clear                 // clear
};