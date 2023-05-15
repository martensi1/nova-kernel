#include "io.h"
#include <stdint.h>
#include <string.h>


#define VGA_TEXT_MODE_WIDTH 80
#define VGA_TEXT_MODE_HEIGHT 25
#define VGA_TEXT_MODE_MEMORY_ADDRESS 0xB8000


static const size_t VGA_WIDTH = VGA_TEXT_MODE_WIDTH;
static const size_t VGA_HEIGHT = VGA_TEXT_MODE_HEIGHT;

static uint16_t* vga_buffer;
static size_t vga_column;
static size_t vga_row;
static uint8_t vga_write_color;


extern "C" {
    static inline uint8_t vga_entry_color(enum vga_color foreground, enum vga_color background) 
    {
        return foreground | background << 4;
    }
    
    static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
    {
        return (uint16_t)uc | (uint16_t)color << 8;
    }


    static void terminal_scroll_up() {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                const size_t index = y * VGA_WIDTH + x;
                const size_t prev_index = (y - 1) * VGA_WIDTH + x;
                vga_buffer[prev_index] = vga_buffer[index];
            }
        }
    }

    static void terminal_put_linefeed() 
    {
        vga_column = 0;

        if (vga_row == VGA_HEIGHT) {
            terminal_scroll_up();
        }
        else {
            vga_row++;
        }
    }

    static void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) 
    {
        const size_t index = y * VGA_WIDTH + x;
        vga_buffer[index] = vga_entry(c, color);
    }

    static void terminal_put_char(char c) 
    {
        if (c == '\n') {
            terminal_put_linefeed();
            return;
        }
    
        terminal_put_entry_at(c, vga_write_color, vga_column, vga_row);

        if (++vga_column == VGA_WIDTH) {
            terminal_put_linefeed();
        }
    }


    void terminal_set_color(vga_color foreground, vga_color background)
    {
        vga_write_color = vga_entry_color(foreground, background);
    }

    void terminal_clear(void)
    {
        for (size_t y = 0; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                terminal_put_entry_at(' ', vga_write_color, x, y);
            }
        }
    }
    
    void terminal_write(const char* data, size_t size)
    {
        for (size_t i = 0; i < size; i++) {
            char c  = data[i];
            terminal_put_char(c);
        }
    }

    void terminal_write_str(const char* data)
    {
        size_t length = strlen(data);
        terminal_write(data, length);
    }

    void terminal_init(void)
    {
        vga_column = 0;
        vga_row = 0;

        vga_buffer = (uint16_t*)VGA_TEXT_MODE_MEMORY_ADDRESS;
        
        terminal_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        terminal_clear();
    }
}