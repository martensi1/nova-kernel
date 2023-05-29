#ifndef __SIMUX_KEYCODE_H__
#define __SIMUX_KEYCODE_H__

#include <simux/types.h>


enum keycode
{
    KEY_ESC = 0x01,
    KEY_NUM1 = 0x02,
    KEY_NUM2 = 0x03,
    KEY_NUM3 = 0x04,
    KEY_NUM4 = 0x05,
    KEY_NUM5 = 0x06,
    KEY_NUM6 = 0x07,
    KEY_NUM7 = 0x08,
    KEY_NUM8 = 0x09,
    KEY_NUM9 = 0x0A,
    KEY_NUM0 = 0x0B,

    KEY_Q = 0x10,
    KEY_W = 0x11,
    KEY_E = 0x12,
    KEY_R = 0x13,
    KEY_T = 0x14,
    KEY_Y = 0x15,
    KEY_U = 0x16,
    KEY_I = 0x17,
    KEY_O = 0x18,
    KEY_P = 0x19,

    KEY_UNKNOWN = 0xFF

};


keycode keycode_from_scan(u8 scancode);

char keycode_to_char(keycode key);


#endif // __SIMUX_KEYCODE_H__