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
#include "serial.h"
#include <nova/cpu/sysbus.h>
#include <nova/kernel.h>


using namespace nova;


SerialPort Serial::ports_[8] = {
    SerialPort(0x3F8), // COM1
    SerialPort(0x2F8), // COM2
    SerialPort(0x3E8), // COM3
    SerialPort(0x2E8), // COM4
    SerialPort(0x5F8), // COM5
    SerialPort(0x4F8), // COM6
    SerialPort(0x5E8), // COM7
    SerialPort(0x4E8)  // COM8
};


void Serial::scanForPorts()
{
    for (u16 i = 0; i < ARRAY_SIZE(ports_); i++) {
        auto comPort = &ports_[i];

        if (comPort->initialize()) {
            Log("Serial port COM%d found, baudrate=%d", (i+1), comPort->getBaudRate());
        }
    }
}

bool Serial::isPortAvailable(Port port)
{
    auto comPort = getPort(port);
    return comPort->isAvailable();
}

void Serial::setup(Serial::Port port)
{
    auto comPort = getPort(port);

    if (!comPort->initialize()) {
        EnterPanic("Failed to initialize serial port", port);
    }

    Log("Serial port COM%d initialized, baudrate=%d", port, comPort->getBaudRate(), comPort);
}

void Serial::writeData(Serial::Port port, char byte)
{
    auto comPort = getPort(port);
    comPort->writeData(byte);
}

void Serial::flush(Serial::Port port)
{
    auto comPort = getPort(port);
    comPort->flush();
}

bool Serial::hasData(Serial::Port port)
{
    auto comPort = getPort(port);
    return comPort->hasData();
}

char Serial::readData(Serial::Port port)
{
    auto comPort = getPort(port);
    return comPort->readData();
}

