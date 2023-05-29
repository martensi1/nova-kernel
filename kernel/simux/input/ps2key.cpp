#include <simux/input/keydrv.h>
#include <simux/input/keycode.h>
#include <simux/cpu/isr.h>
#include <simux/cpu/sysbus.h>
#include <simux/kernel.h>


#define PS2KEY_DATA_PORT 0x60
static key_handler on_key_pressed_callback = NULL;


// -- Private --
static void on_key_pressed(const u8 scancode)
{
    if (on_key_pressed_callback != NULL)
    {
        on_key_pressed_callback(scancode);
    }
}


static bool ps2key_is_available()
{
    return true;
}

static void ps2key_initialize()
{
    isr_add_interrupt_handler(IRQ1, (irq_handler)on_key_pressed);
}

static void ps2key_on_key_pressed(key_handler callback)
{
    on_key_pressed_callback = callback;
}




// -- Public --

struct keyboard_driver ps2key_driver = {
    "ps2key",                // name
    ps2key_is_available,     // is_available
    ps2key_initialize,       // initialize
    ps2key_on_key_pressed    // on_key_pressed
};
