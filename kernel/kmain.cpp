#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include "io.h"


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif


extern "C" {
    void kmain() {
        terminal_init();
        terminal_write_str("Hello, kernel World!\n");
        terminal_write_str("This is a new line\n");

        while (true) {

        }
    }
}