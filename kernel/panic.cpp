#include <simux/panic.h>
#include <simux/term.h>


static void khalt(void)
{
    while (true) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}

void kpanic(const char* message)
{
    asm volatile("cli");

    term_clear();
    term_write_str("Kernel panic!\n");

    term_write_str("Message: ");
    term_write_str(message);
    term_write_str("\n");

    khalt();
}