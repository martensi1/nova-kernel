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
#include <nova/drivers/console.h>
#include <arch/sysbus.h>


using namespace nova;


#define VGA_TEXT_MODE_WIDTH 80
#define VGA_TEXT_MODE_HEIGHT 20
#define VGA_TEXT_BUFFER_ADDRESS 0xB8000

#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

#define VGA_REGISTER_MODE 0x03
#define VGA_REGISTER_CURSOR_START 0x0A
#define VGA_REGISTER_CURSOR_END 0x0B
#define VGA_REGISTER_CURSOR_HIGH 0x0E
#define VGA_REGISTER_CURSOR_LOW 0x0F

#define VGA_CURSOR_START_SCANLINE 0
#define VGA_CURSOR_END_SCANLINE 15


static const size_t VGA_WIDTH = VGA_TEXT_MODE_WIDTH;
static const size_t VGA_HEIGHT = VGA_TEXT_MODE_HEIGHT;

static u16* videoBuffer;
static u8 writeColor;

static size_t column;
static size_t row;


enum VGAColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,   
};


namespace priv
{
    static inline u8 ReadRegister(u8 index)
    {
        WriteIO(VGA_INDEX_PORT, index);
        return ReadIO(VGA_DATA_PORT);
    }

    static inline void WriteRegister(u8 index, u8 value)
    {
        WriteIO(VGA_INDEX_PORT, index);
        WriteIO(VGA_DATA_PORT, value);
    }


    static inline void SetOperationMode(u8 mode)
    {
        WriteRegister(VGA_REGISTER_MODE, mode);
    }

    static inline void WriteVideoMemory(char c, u8 color, size_t x, size_t y)
    {
        const size_t offset = y * VGA_WIDTH + x;
        u16 byte = (u16)c | (u16)color << 8;

        videoBuffer[offset] = byte;
    }

    static inline void SetCursorPos(size_t x, size_t y)
    {
        size_t offset = y * VGA_WIDTH + x;

        WriteRegister(VGA_REGISTER_CURSOR_HIGH, (u8)(offset >> 8));
        WriteRegister(VGA_REGISTER_CURSOR_LOW, (u8)(offset & 0xFF));
    }

    static inline void EnableCursor(u8 cursorStart, u8 cursorEnd)
    {
        u8 startValue = ReadRegister(VGA_REGISTER_CURSOR_START);
        u8 endValue = ReadRegister(VGA_REGISTER_CURSOR_END);

        startValue = (startValue & 0xC0) | (cursorStart & 0x0F);
        endValue = (endValue & 0xE0) | (cursorEnd & 0x0F);

        WriteRegister(VGA_REGISTER_CURSOR_START, startValue);
        WriteRegister(VGA_REGISTER_CURSOR_END, endValue);
    }

    static inline void DisableCursor(void)
    {
        u8 value = 0x01 << 5;
        WriteRegister(VGA_REGISTER_CURSOR_START, value);
    }

    static void ScrollUp(void) {
        for (size_t y = 0; y < (VGA_HEIGHT - 1); y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                const size_t index = y * VGA_WIDTH + x;
                const size_t nextIndex = (y + 1) * VGA_WIDTH + x;

                videoBuffer[index] = videoBuffer[nextIndex];
                videoBuffer[nextIndex] = 0;
            }
        }

        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            videoBuffer[index] = 0;
        }
    }

    static void SetWriteColor(VGAColor foreground, VGAColor background)
    {
        u8 colorByte = foreground | background << 4;
        writeColor = colorByte;
    }
}

bool IsAvailable(void)
{
    return true;
}

static void Initialize(void)
{
    column = 0;
    row = 0;

    videoBuffer = (u16*)VGA_TEXT_BUFFER_ADDRESS;
    
    priv::SetOperationMode(0x03);
    priv::SetWriteColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

static void WriteLineFeed(void)
{
    column = 0;

    if (row == (VGA_HEIGHT - 1)) {
        priv::ScrollUp();
    }
    else {
        row++;
    }
}

static void WriteChar(const char c)
{
    priv::WriteVideoMemory(c, writeColor, column, row);

    if (++column == VGA_WIDTH) {
        WriteLineFeed();
    }
}

static void EnableCursor(void)
{
    priv::EnableCursor(
        VGA_CURSOR_START_SCANLINE,
        VGA_CURSOR_END_SCANLINE
    );
}

static void UpdateCursor(void)
{
    priv::SetCursorPos(column, row);
}

static void DisableCursor(void)
{
    priv::DisableCursor();
}

static void Flush(void)
{
    // Do nothing
}

static void Clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            priv::WriteVideoMemory(' ', writeColor, x, y);
        }
    }

    column = 0;
    row = 0;
}

// Define driver
console_driver_t vgaDriver = {
    "vgacon",        // name
    IsAvailable,     // is_available
    Initialize,      // initialize
    WriteChar,       // write_char
    WriteLineFeed,   // write_line_feed
    EnableCursor,    // enable_cursor
    UpdateCursor,    // update_cursor
    DisableCursor,   // disable_cursor
    Flush,           // flush
    Clear            // clear
};