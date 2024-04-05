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
#ifndef NOVA_SERIAL_PORT_H
#define NOVA_SERIAL_PORT_H

#include <nova/types.h>
#include <nova/sync/spin_lock.h>

class SerialPort
{
public:
    SerialPort(u16 ioPort, u8 divisor = 1);
    ~SerialPort();

    bool initialize();

    void writeData(u8 byte);
    void flush();

    bool hasData();
    u8 readData();

    u32 getPort() const;
    u32 getBaudRate() const;
    
    bool isAvailable() const;

private:
    bool doSelfTest();

    void enterLoopbackMode();
    void exitLoopbackMode();

    bool isTransitEmpty();

    bool available_;

    u16 ioPort_;
    u8 divisor_;

    u8 buffer_[256];
    u32 index_;

    spin_lock lock_;
};

#endif // NOVA_SERIAL_PORT_H