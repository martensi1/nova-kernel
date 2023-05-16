#ifndef __SIMUX_CONSOLE_DRIVER_H__
#define __SIMUX_CONSOLE_DRIVER_H__

#include <stddef.h>


struct console_driver {
    void (*initialize)();
    void (*write_char)(const char c);
    void (*write_line_feed)();
    void (*enable_cursor)();
    void (*update_cursor)();
    void (*disable_cursor)();
    void (*clear)();
};


#endif // __SIMUX_CONSOLE_DRIVER_H__