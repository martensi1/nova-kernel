#ifndef __SIMUX_CONSOLE_DRIVER_H__
#define __SIMUX_CONSOLE_DRIVER_H__


struct console_driver {
    char name[10];
    
    bool (*is_available)();
    void (*initialize)();
    void (*write_char)(const char c);
    void (*write_line_feed)();
    void (*enable_cursor)();
    void (*update_cursor)();
    void (*disable_cursor)();
    void (*clear)();
};


#endif // __SIMUX_CONSOLE_DRIVER_H__