/*
Dummy console driver
Used if no other console driver is available
*/
#include <simux/cpu/sysbus.h>
#include <simux/drivers/condrv.h>
#include <stdint.h>
#include <string.h>


bool dummy_con_is_available(void)
{
    return true;
}

void dummy_con_initialize(void)
{
}

void dummy_con_write_line_feed(void)
{
}

void dummy_con_write_char(const char)
{
}

void dummy_con_enable_cursor(void)
{
}

void dummy_con_update_cursor(void)
{
}

void dummy_con_disable_cursor(void)
{
}

void dummy_con_clear(void)
{
}


// Define driver
struct console_driver dummy_driver = {
    "dummycon",                 // name
    dummy_con_is_available,     // is_available
    dummy_con_initialize,       // initialize
    dummy_con_write_char,       // write_char
    dummy_con_write_line_feed,  // write_line_feed
    dummy_con_enable_cursor,    // enable_cursor
    dummy_con_update_cursor,    // update_cursor
    dummy_con_disable_cursor,   // disable_cursor
    dummy_con_clear             // clear
};