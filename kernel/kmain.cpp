#include <nova/tty.h>
#include <nova/cpu/setup.h>
#include <nova/kernel.h>
#include <nova/hbit.h>
#include <nova/sbit.h>
#include <nova/timers/pit.h>
#include <nova/build.h>
#include <nova/serial/serial.h>

using namespace Nova;


extern "C" {
    void kmain(u32 boot_handover_eax) 
    {
        term_initialize();

        Log("==============================");
        Log("= NOVA OS");
        Log("==============================");
        Log("%s built %s by %s", NOVA_ARCHITECTURE, NOVA_BUILD_DATETIME, NOVA_COMPILER);
        
        RunHBIT(boot_handover_eax);
        pic_setup_interrupt_generator(200);
        cpu_setup();

        Serial::setup(Serial::COM1);
        //Serial::setup(Serial::COM2);
        Serial::writeData(Serial::COM1, 'H');
        Serial::writeData(Serial::COM1, 'H');
        //Serial::writeData(Serial::COM2, 'H');
        //Serial::writeData(Serial::COM2, 'H');
        Serial::flush(Serial::COM1);
        //Serial::flush(Serial::COM2);

        RunSBIT();

        while (true) {
            asm volatile("hlt");
        }
    }
}