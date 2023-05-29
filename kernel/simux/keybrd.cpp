#include <simux/keybrd.h>
#include <simux/input/keydrv.h>
#include <simux/input/keycode.h>
#include <simux/kernel.h>





extern struct keyboard_driver ps2key_driver;
struct keyboard_driver* used_driver = &ps2key_driver;

static void on_key_pressed(const u8 scancode)
{
    keycode code = keycode_from_scan(scancode);
    char key = keycode_to_char(code);

    static_cast<void>(key);

    logk("Key pressed %d\n", scancode);
}



void keyboard_initialize(void)
{
    used_driver->initialize();
    used_driver->on_key_pressed(on_key_pressed);
}


