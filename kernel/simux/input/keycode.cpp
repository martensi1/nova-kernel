#include <simux/input/keycode.h>



keycode keycode_from_scan(u8 scancode)
{
    switch (scancode)
    {
        case 0x01:
            return KEY_ESC;
        case 0x02:
            return KEY_NUM1;
        case 0x03:
            return KEY_NUM2;
        case 0x04:
            return KEY_NUM3;
        case 0x05:
            return KEY_NUM4;
        case 0x06:
            return KEY_NUM5;
        case 0x07:
            return KEY_NUM6;
        case 0x08:
            return KEY_NUM7;
        case 0x09:
            return KEY_NUM8;
        case 0x0A:
            return KEY_NUM9;
        case 0x0B:
            return KEY_NUM0;
        case 0x10:
            return KEY_Q;
        case 0x11:
            return KEY_W;
        case 0x12:
            return KEY_E;
        case 0x13:
            return KEY_R;
        case 0x14:
            return KEY_T;
        case 0x15:
            return KEY_Y;
        case 0x16:
            return KEY_U;
        case 0x17:
            return KEY_I;
        case 0x18:
            return KEY_O;
        case 0x19:
            return KEY_P;
        default:
            return KEY_UNKNOWN;
    }
}

char keycode_to_char(keycode code)
{
    switch (code)
    {
        case KEY_NUM1:
            return '1';
        case KEY_NUM2:
            return '2';
        case KEY_NUM3:
            return '3';
        case KEY_NUM4:
            return '4';
        case KEY_NUM5:
            return '5';
        case KEY_NUM6:
            return '6';
        case KEY_NUM7:
            return '7';
        case KEY_NUM8:
            return '8';
        case KEY_NUM9:
            return '9';
        case KEY_NUM0:
            return '0';
        case KEY_Q:
            return 'q';
        case KEY_W:
            return 'w';
        case KEY_E:
            return 'e';
        case KEY_R:
            return 'r';
        case KEY_T:
            return 't';
        case KEY_Y:
            return 'y';
        case KEY_U:
            return 'u';
        case KEY_I:
            return 'i';
        default:
            return '?';
    }
}