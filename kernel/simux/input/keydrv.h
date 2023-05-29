#ifndef __SIMUX_KEYBOARD_DRIVER_H__
#define __SIMUX_KEYBOARD_DRIVER_H__

#include <simux/types.h>

typedef void(*key_handler)(const u8 scancode);


struct keyboard_driver {
    char name[10];
    
    bool (*is_available)();
    void (*initialize)();
    void (*on_key_pressed)(key_handler callback);
};


#endif // __SIMUX_KEYBOARD_DRIVER_H__