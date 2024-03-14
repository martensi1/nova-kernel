#include <nova/tty.h>
#include <nova/cpu/setup.h>
#include <nova/kernel.h>
#include <nova/hbit.h>
#include <nova/sbit.h>
#include <nova/timers/pit.h>
#include <nova/build.h>


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

        logk("==============================");
        logk("= NOVA OS");
        logk("==============================");
        logk("%s built %s by %s", NOVA_ARCHITECTURE, NOVA_BUILD_DATETIME, NOVA_COMPILER);
        
        hbit_run(boot_handover_eax);
        pic_setup_interrupt_generator(200);
        cpu_setup();

        sbit_run();

        while (true) {
            asm volatile("hlt");
        }
    }
}