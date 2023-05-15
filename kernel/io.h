#ifndef __SIMUX_IO_H__
#define __SIMUX_IO_H__

#include <stddef.h>


extern "C" {
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

    void terminal_init(void);

    void terminal_set_color(vga_color foreground, vga_color background);
    void terminal_clear(void);

    void terminal_write(const char* data, size_t size);
    void terminal_write_str(const char* data);
    
}

#endif // __SIMUX_IO_H__