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
#include <nova/tty.h>

#include <nova/drivers/console.h>
#include <nova/types/cbuffer.h>
#include <nova/sync/spin_guard.h>
#include <libc/string.h>


extern console_driver_t vgaDriver;
extern console_driver_t serialDriver;

static CircularBuffer<1024> history;

static TerminalInterface interfaces[2] = {
    { "Screen", &vgaDriver },
    { "Serial", &serialDriver }
};



////////////////////////////////////////////////////////////
TerminalInterface::TerminalInterface(const char* name, console_driver_t* driver)
    : name_(name)
    , console_(driver)
{
}

////////////////////////////////////////////////////////////
bool TerminalInterface::Enable()
{
    spin_guard guard(lock_);

    if (enabled_) {
        return false;
    }

    console_->initialize();
    console_->clear();

    console_->enableCursor();
    console_->updateCursor();

    writeData("Terminal initialized (");
    writeData(console_->name);
    writeData(")\n");

    enabled_ = true;
    return true;
}

////////////////////////////////////////////////////////////
void TerminalInterface::Disable()
{
    spin_guard guard(lock_);

    if (!enabled_) {
        return;
    }

    console_->clear();
    console_->disableCursor();
    enabled_ = false;
}

////////////////////////////////////////////////////////////
void TerminalInterface::Write(const char* data, size_t size)
{
    spin_guard guard(lock_);
    writeData(data, size);
}


////////////////////////////////////////////////////////////
void TerminalInterface::Write(const char* str)
{
    size_t length = strlen(str);
    writeData(str, length);
}

////////////////////////////////////////////////////////////
void TerminalInterface::Flush()
{
    spin_guard guard(lock_);
    console_->flush();
}

////////////////////////////////////////////////////////////
void TerminalInterface::Clear()
{
    spin_guard guard(lock_);
    console_->clear();
}

////////////////////////////////////////////////////////////
void TerminalInterface::writeData(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        char c  = data[i];

        if (c == '\n') {
            console_->writeLineFeed();
            continue;
        }

        console_->writeChar(c);
    }

    console_->updateCursor();
    console_->flush();
}

////////////////////////////////////////////////////////////
void TerminalInterface::writeData(const char* str)
{
    size_t length = strlen(str);
    writeData(str, length);
}


void terminal_scan_drivers()
{
    ARRAY_FOR_EACH(i, interfaces) {
        auto interface = &interfaces[i];

        if (interface->IsAvailable()) {
            bool enabled = interface->Enable();

            if (enabled) {
                char buffer[1024];
                history.read(buffer);
                interface->Write(buffer, history.size());
            }
        }
        else if (!interface->IsAvailable()) {
            interface->Disable();
        }
    }
}

void terminal_clear()
{
    ARRAY_FOR_EACH(i, interfaces) {
        auto interface = &interfaces[i];

        if (interface->IsEnabled()) {
            interface->Clear();
        }
    }

    history.clear();
}

void terminal_write(const char* data, size_t size)
{
    history.push(data, size);

    ARRAY_FOR_EACH(i, interfaces) {
        auto interface = &interfaces[i];

        if (interface->IsEnabled()) {
            interface->Write(data, size);
        }
    }
}

void terminal_write(const char* str)
{
    size_t length = strlen(str);
    terminal_write(str, length);
}

void terminal_flush()
{
    ARRAY_FOR_EACH(i, interfaces) {
        auto interface = &interfaces[i];

        if (interface->IsEnabled()) {
            interface->Flush();
        }
    }
}
