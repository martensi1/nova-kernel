#include <simux/tty.h>
#include <simux/cpu/setup.h>
#include <simux/kernel.h>
#include <simux/hbit.h>


#if !defined(__i386__)
    #error This project must be compiled with an x86-elf compiler!
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
    #error This project must be compiled with a little endian compiler!
#endif


extern "C" {
    void kmain(u32 boot_handover_eax) 
    {
        term_initialize();
        
        hbit_run(boot_handover_eax);
        cpu_setup();

        while (true) {
            asm volatile("hlt");
        }
    }
}