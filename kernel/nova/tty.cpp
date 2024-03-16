#include <nova/tty.h>
#include <nova/spinlock.h>
#include <nova/console/driver.h>
#include <libc/string.h>


struct console_driver* current_driver = NULL;
static SpinLock write_lock = SpinLock();


static void choose_driver()
{
    extern struct console_driver dummy_driver;
    extern struct console_driver vga_driver;

    if (vga_driver.is_available()) {
        current_driver = &vga_driver;
    }
    else {
        current_driver = &dummy_driver;
    }
}


// -- Public functions --

void term_initialize()
{
    choose_driver();

    current_driver->initialize();
    current_driver->clear();

    current_driver->enable_cursor();
    current_driver->update_cursor();

    term_write_str("\n\n\n\n\n");
    term_write_str("Terminal initialized (");
    term_write_str(current_driver->name);
    term_write_str(")\n");
}

void term_clear()
{
    current_driver->clear();
}

void term_write(const char* data, size_t size)
{
    write_lock.aqquire();

    for (size_t i = 0; i < size; i++) {
        char c  = data[i];

        if (c == '\n') {
            current_driver->write_line_feed();
            continue;
        }

        current_driver->write_char(c);
    }

    current_driver->update_cursor();
    write_lock.release();
}

void term_write_str(const char* str)
{
    size_t length = strlen(str);
    term_write(str, length);
}