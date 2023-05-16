#include <simux/term.h>
#include <simux/string.h>


extern struct console_driver vga_driver;


void term_initialize() {
    vga_driver.initialize();
    vga_driver.disable_cursor();
    vga_driver.clear();
}

void term_clear() {
    vga_driver.clear();
}

void term_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        char c  = data[i];

        if (c == '\n') {
            vga_driver.write_line_feed();
            continue;
        }

        vga_driver.write_char(c);
    }

    vga_driver.update_cursor();
}

void term_write_str(const char* str) {
    size_t length = strlen(str);
    term_write(str, length);
}