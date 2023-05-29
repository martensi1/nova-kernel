/**
 * vgamode.cpp
 * VGA mode console driver
*/
#include <simux/console/driver.h>
#include <simux/cpu/sysbus.h>


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

static u16* vga_buffer;
static size_t vga_column;
static size_t vga_row;
static u8 vga_write_color;


enum vga_color {
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



static inline u8 read_register(u8 index)
{
    sysbus_io_out(VGA_INDEX_PORT, index);
    return sysbus_io_in(VGA_DATA_PORT);
}

static inline void write_register(u8 index, u8 value)
{
    sysbus_io_out(VGA_INDEX_PORT, index);
    sysbus_io_out(VGA_DATA_PORT, value);
}


static inline void set_operation_mode(u8 mode)
{
    write_register(VGA_REGISTER_MODE, mode);
}

static inline void write_video_memory(char c, u8 color, size_t x, size_t y)
{
    const size_t offset = y * VGA_WIDTH + x;
    u16 byte = (u16)c | (u16)color << 8;

    vga_buffer[offset] = byte;
}

static inline void set_cursor_pos(size_t x, size_t y)
{
    size_t offset = y * VGA_WIDTH + x;

    write_register(VGA_REGISTER_CURSOR_HIGH, (u8)(offset >> 8));
    write_register(VGA_REGISTER_CURSOR_LOW, (u8)(offset & 0xFF));
}

static inline void enable_cursor(u8 cursor_start, u8 cursor_end)
{
    u8 start_value = read_register(VGA_REGISTER_CURSOR_START);
    u8 end_value = read_register(VGA_REGISTER_CURSOR_END);

    start_value = (start_value & 0xC0) | (cursor_start & 0x0F);
    end_value = (end_value & 0xE0) | (cursor_end & 0x0F);

    write_register(VGA_REGISTER_CURSOR_START, start_value);
    write_register(VGA_REGISTER_CURSOR_END, end_value);
}

static inline void disable_cursor(void)
{
    u8 value = 0x01 << 5;
    write_register(VGA_REGISTER_CURSOR_START, value);
}

static void scroll_up(void) {
    for (size_t y = 0; y < (VGA_HEIGHT - 1); y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            const size_t next_index = (y + 1) * VGA_WIDTH + x;
            vga_buffer[index] = vga_buffer[next_index];
            vga_buffer[next_index] = 0;
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        vga_buffer[index] = 0;
    }
}

static void set_write_color(vga_color foreground, vga_color background)
{
    u8 color_byte = foreground | background << 4;
    vga_write_color = color_byte;
}



bool vga_is_available(void)
{
    return true;
}

static void vga_initialize(void)
{
    vga_column = 0;
    vga_row = 0;

    vga_buffer = (u16*)VGA_TEXT_BUFFER_ADDRESS;
    
    set_operation_mode(0x03);
    set_write_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

static void vga_write_line_feed(void)
{
    vga_column = 0;

    if (vga_row == (VGA_HEIGHT - 1)) {
        scroll_up();
    }
    else {
        vga_row++;
    }
}

static void vga_write_char(const char c)
{
    write_video_memory(c, vga_write_color, vga_column, vga_row);

    if (++vga_column == VGA_WIDTH) {
        vga_write_line_feed();
    }
}

static void vga_enable_cursor(void)
{
    enable_cursor(
        VGA_CURSOR_START_SCANLINE,
        VGA_CURSOR_END_SCANLINE
    );
}

static void vga_update_cursor(void)
{
    set_cursor_pos(vga_column, vga_row);
}

static void vga_disable_cursor(void)
{
    disable_cursor();
}

static void vga_clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            write_video_memory(' ', vga_write_color, x, y);
        }
    }

    vga_column = 0;
    vga_row = 0;
}


// Define driver
struct console_driver vga_driver = {
    "vgacon",             // name
    vga_is_available,     // is_available
    vga_initialize,       // initialize
    vga_write_char,       // write_char
    vga_write_line_feed,  // write_line_feed
    vga_enable_cursor,    // enable_cursor
    vga_update_cursor,    // update_cursor
    vga_disable_cursor,   // disable_cursor
    vga_clear             // clear
};