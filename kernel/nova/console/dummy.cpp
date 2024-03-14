/**
 * dummy.cpp
 * Dummy console driver, used if no other console driver is available
*/
#include <nova/console/driver.h>
#include <nova/cpu/sysbus.h>


static bool dummy_con_is_available(void)
{
    return true;
}

static void dummy_con_initialize(void)
{
}

static void dummy_con_write_line_feed(void)
{
}

static void dummy_con_write_char(const char)
{
}

static void dummy_con_enable_cursor(void)
{
}

static void dummy_con_update_cursor(void)
{
}

static void dummy_con_disable_cursor(void)
{
}

static void dummy_con_clear(void)
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