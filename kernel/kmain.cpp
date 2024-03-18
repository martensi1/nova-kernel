#include <nova/tty.h>
#include <nova/cpu/setup.h>
#include <nova/kernel.h>
#include <nova/hbit.h>
#include <nova/sbit.h>
#include <nova/pit.h>
#include <nova/build.h>
#include <nova/serial/serial.h>

using namespace Nova;


extern "C" {
    void kmain(u32 boot_handover_eax) 
    {
        InitializeTerminal();

        Log("==============================");
        Log("= NOVA OS");
        Log("==============================");
        Log("%s built %s by %s", NOVA_ARCHITECTURE, NOVA_BUILD_DATETIME, NOVA_COMPILER);
        
        RunHBIT(boot_handover_eax);

        Serial::scanForPorts();
        TerminalScanDrivers();
        
        SetupPIC(200);
        cpu_setup();

        RunSBIT();

        while (true) {
            asm volatile("hlt");
        }
    }
}