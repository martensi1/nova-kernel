#include <simux/panic.h>
#include <simux/term.h>
#include <simux/stdio.h>


static void khalt(void)
{
    while (true) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}

void kpanic(const char* message, uint32_t data)
{
    asm volatile("cli");

    term_clear();

    printf("Kernel panic!\n");
    printf("Message: %s\n", message);
    printf("Data: 0x%x\n", data);

    khalt();
}