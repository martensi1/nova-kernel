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
#ifndef NOVA_SERIAL_H
#define NOVA_SERIAL_H

#include "serial_port.h"
#include <nova/types.h>

class Serial
{
public:
    enum Port : u16 {
        COM1 = 0x01,
        COM2 = 0x02,
        COM3 = 0x03,
        COM4 = 0x04,
        COM5 = 0x05,
        COM6 = 0x06,
        COM7 = 0x07,
        COM8 = 0x08,
    };

    static void scanForPorts();
    static bool isPortAvailable(Port port);

    static void setup(Port port);

    static void writeData(Port port, char byte);
    static void flush(Port port);

    static bool hasData(Port port);
    static char readData(Port port);

private:
    static inline SerialPort* getPort(Port port) { 
        return &ports_[port - 1]; 
    }

    static SerialPort ports_[8];

};

#endif // NOVA_SERIAL_H