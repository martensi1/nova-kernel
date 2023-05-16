/*
VGA console driver
http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
*/
#include <simux/vgacon.h>
#include <simux/sysbus.h>
#include <simux/atoi.h>
#include <stdint.h>
#include <string.h>


#define VGA_TEXT_MODE_WIDTH 80
#define VGA_TEXT_MODE_HEIGHT 25
#define VGA_TEXT_BUFFER_ADDRESS 0xB8000

#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

#define VGA_REGISTER_MODE 0x03
#define VGA_REGISTER_CURSOR_START 0x0A
#define VGA_REGISTER_CURSOR_END 0x0B
#define VGA_REGISTER_CURSOR_HIGH 0x0E
#define VGA_REGISTER_CURSOR_LOW 0x0F


static const size_t VGA_WIDTH = VGA_TEXT_MODE_WIDTH;
static const size_t VGA_HEIGHT = VGA_TEXT_MODE_HEIGHT;

static uint16_t* vga_buffer;
static size_t vga_column;
static size_t vga_row;
static uint8_t vga_write_color;


extern "C" {
    static inline uint8_t read_register(uint8_t index)
    {
        sysbus_io_out(VGA_INDEX_PORT, index);
        return sysbus_io_in(VGA_DATA_PORT);
    }

    static inline void write_register(uint8_t index, uint8_t value)
    {
        sysbus_io_out(VGA_INDEX_PORT, index);
        sysbus_io_out(VGA_DATA_PORT, value);
    }

    static inline void write_video_memory(char c, uint8_t color, size_t x, size_t y)
    {
        const size_t offset = y * VGA_WIDTH + x;
        uint16_t byte = (uint16_t)c | (uint16_t)color << 8;

        vga_buffer[offset] = byte;
    }

    static inline void set_mode(uint8_t mode)
    {
        write_register(VGA_REGISTER_MODE, mode);
    }

    static inline void set_cursor_pos(size_t x, size_t y)
    {
        size_t offset = y * VGA_WIDTH + x;

        write_register(VGA_REGISTER_CURSOR_HIGH, (uint8_t)(offset >> 8));
        write_register(VGA_REGISTER_CURSOR_LOW, (uint8_t)(offset & 0xFF));
    }

    static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
    {
        //uint8_t start_value = (0x00 << 5) | (0x0F & cursor_start);
        //uint8_t end_value = (0x0F & cursor_end);
        uint8_t start_value = read_register(VGA_REGISTER_CURSOR_START);
        uint8_t end_value = read_register(VGA_REGISTER_CURSOR_END);

        start_value = (start_value & 0xC0) | cursor_start;
        end_value = (end_value & 0xE0) | cursor_end;

        write_register(VGA_REGISTER_CURSOR_START, start_value);
        write_register(VGA_REGISTER_CURSOR_END, end_value);
    }

    static void disable_cursor(void)
    {
        uint8_t value = 0x01 << 5;
        write_register(VGA_REGISTER_CURSOR_START, value);
    }


    static void scroll_up() {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                const size_t index = y * VGA_WIDTH + x;
                const size_t prev_index = (y - 1) * VGA_WIDTH + x;
                vga_buffer[prev_index] = vga_buffer[index];
            }
        }
    }

    static void put_line_feed() 
    {
        vga_column = 0;

        if (vga_row == VGA_HEIGHT && false) {
            scroll_up();
        }
        else {
            vga_row++;
        }
    }

    static void put_entry(char c) 
    {
        if (c == '\n') {
            put_line_feed();
            return;
        }
    
        write_video_memory(c, vga_write_color, vga_column, vga_row);

        if (++vga_column == VGA_WIDTH) {
            put_line_feed();
        }
    }
    


    void vga_initialize(void)
    {
        vga_column = 0;
        vga_row = 0;

        vga_buffer = (uint16_t*)VGA_TEXT_BUFFER_ADDRESS;
        
        set_mode(0x03);
        disable_cursor();
        enable_cursor(0x0A, 0x0F);
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        vga_clear();
    }
    
    void vga_write(const char* data, size_t size)
    {
        for (size_t i = 0; i < size; i++) {
            char c  = data[i];
            put_entry(c);
        }
    }

    void vga_write_str(const char* data)
    {
        size_t length = strlen(data);
        vga_write(data, length);
        set_cursor_pos(10, 4);
        
    }

    void vga_set_color(vga_color foreground, vga_color background)
    {
        uint8_t color_byte = foreground | background << 4;
        vga_write_color = color_byte;
    }

    void vga_clear(void)
    {
        for (size_t y = 0; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                write_video_memory(' ', vga_write_color, x, y);
            }
        }
    }
}