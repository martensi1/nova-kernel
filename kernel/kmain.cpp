#include <simux/vgacon.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif


extern "C" {
    void kmain() {
        vga_initialize();
        vga_write_str("\n\n");

        vga_write_str("Using VGA text mode\n");
        vga_write_str("Loading Simux kernel...\n");

        while (true) {

        }
    }
}