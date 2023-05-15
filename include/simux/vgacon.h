#ifndef __SIMUX_VGA_CON_H__
#define __SIMUX_VGA_CON_H__

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

    void vga_initialize(void);

    void vga_write(const char* data, size_t size);
    void vga_write_str(const char* data);

    void vga_set_color(vga_color foreground, vga_color background);
    void vga_clear(void);
}

#endif // __SIMUX_VGA_CON_H__