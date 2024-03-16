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
#include "serial_port.h"
#include <nova/cpu/sysbus.h>
#include <nova/kernel.h>


SerialPort::SerialPort(u16 ioPort, u8 divisor) :
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
    lock_.aqquire();

    if (divisor_ > 3 || divisor_ < 1)
    {
        kpanic("Invalid divisor for serial port", divisor_);
        return false;
    }

    sysbus_io_out(ioPort_ + 1, 0x00);     // Disable all interrupts
    sysbus_io_out(ioPort_ + 3, 0x80);     // Enable DLAB (for setting the baud rate divisor)
    sysbus_io_out(ioPort_ + 0, divisor_); // Set divisor to 3, i.e 38400 baudrate (lo byte)
    sysbus_io_out(ioPort_ + 1, 0x00);     //                                      (hi byte)
    sysbus_io_out(ioPort_ + 3, 0x03);     // 8 bits, no parity, one stop bit
    sysbus_io_out(ioPort_ + 2, 0xC7);     // Enable FIFO, clear them, with 14-byte threshold
    sysbus_io_out(ioPort_ + 4, 0x0B);     // IRQs enabled, RTS/DSR set

    lock_.release();

    return true;
}

bool SerialPort::doSelfTest()
{
    lock_.aqquire();

    enterLoopbackMode();
    u8 testByte = 0xAE;

    writeData(testByte);
    flush();

    bool result = (readData() == testByte);
    exitLoopbackMode();

    lock_.release();

    return result;
}

void SerialPort::writeData(u8 byte)
{
    lock_.aqquire();

    buffer_[index_] = byte;
    index_++;

    if (index_ >= sizeof(buffer_) || byte == '\n')
    {
        flush();
    }

    lock_.release();
}

void SerialPort::flush()
{
    for (u8 i = 0; i < index_; ++i)
    {
        while(!isTransitEmpty());
        sysbus_io_out(ioPort_, buffer_[i]);
    }

    index_ = 0;
}

bool SerialPort::hasData()
{
    return (sysbus_io_in(ioPort_ + 5) & 1) != 0;
}

u8 SerialPort::readData()
{
    while (!hasData());
    return sysbus_io_in(ioPort_);
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

void SerialPort::enterLoopbackMode()
{
    sysbus_io_out(ioPort_ + 4, 0x1E);
}

void SerialPort::exitLoopbackMode()
{
    sysbus_io_out(ioPort_ + 4, 0x0F);
}

bool SerialPort::isTransitEmpty()
{
    return (sysbus_io_in(ioPort_ + 5) & 0x20) != 0;
}