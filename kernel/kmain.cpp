#include <simux/term.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif


extern "C" {
    void kmain() {
        term_initialize();
        term_write_str("\n\n\n");
        term_write_str("Using VGA text mode\n");
        term_write_str("Loading Simux kernel...\n");

        while (true) {

        }
    }
}