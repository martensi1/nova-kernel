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
#include <arch/serial/serial_port.h>
#include <arch/cpu/sysbus.h>
#include <nova/panic.h>
#include <nova/sync/spin_guard.h>

using namespace nova;

SerialPort::SerialPort(u16 ioPort, u8 divisor) :
    available_(false),
    ioPort_(ioPort),
    divisor_(divisor),
    index_(0),
    lock_()
{
}

SerialPort::~SerialPort()
{

}

bool SerialPort::initialize()
{
    spin_guard guard(lock_);

    if (divisor_ > 3 || divisor_ < 1)
    {
        kernel_panic("Invalid divisor for serial port", divisor_);
        return false;
    }

    WriteIO(ioPort_ + 1, 0x00);     // Disable all interrupts
    WriteIO(ioPort_ + 3, 0x80);     // Enable DLAB (for setting the baud rate divisor)
    WriteIO(ioPort_ + 0, divisor_); // Set divisor to 3, i.e 38400 baudrate (lo byte)
    WriteIO(ioPort_ + 1, 0x00);     //                                      (hi byte)
    WriteIO(ioPort_ + 3, 0x03);     // 8 bits, no parity, one stop bit
    WriteIO(ioPort_ + 2, 0xC7);     // Enable FIFO, clear them, with 14-byte threshold
    WriteIO(ioPort_ + 4, 0x0B);     // IRQs enabled, RTS/DSR set

    if (!doSelfTest())
    {
        return false;
    }

    available_ = true;
    return true;
}

void SerialPort::writeData(u8 byte)
{
    buffer_[index_] = byte;
    index_++;

    if (index_ >= sizeof(buffer_) || byte == '\n')
    {
        flush();
    }
}

void SerialPort::flush()
{
    for (u8 i = 0; i < index_; ++i)
    {
        while(!isTransitEmpty());
        WriteIO(ioPort_, buffer_[i]);
    }

    index_ = 0;
}

bool SerialPort::hasData()
{
    return (ReadIO(ioPort_ + 5) & 1) != 0;
}

u8 SerialPort::readData()
{
    while (!hasData());
    return ReadIO(ioPort_);
}

u32 SerialPort::getPort() const
{
    return ioPort_;
}

u32 SerialPort::getBaudRate() const
{
    constexpr u32 baseBaudRate = 115200;
    return baseBaudRate / divisor_;
}

bool SerialPort::isAvailable() const
{
    return available_;
}

bool SerialPort::doSelfTest()
{
    enterLoopbackMode();
    u8 testByte = 0xAE;

    writeData(testByte);
    flush();

    bool result = (readData() == testByte);
    exitLoopbackMode();

    return result;
}


void SerialPort::enterLoopbackMode()
{
    WriteIO(ioPort_ + 4, 0x1E);
}

void SerialPort::exitLoopbackMode()
{
    WriteIO(ioPort_ + 4, 0x0F);
}

bool SerialPort::isTransitEmpty()
{
    return (ReadIO(ioPort_ + 5) & 0x20) != 0;
}