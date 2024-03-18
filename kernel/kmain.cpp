#include <nova/tty.h>
#include <nova/cpu/setup.h>
#include <nova/kernel.h>
#include <nova/hbit.h>
#include <nova/sbit.h>
#include <nova/time/pit.h>
#include <nova/build.h>
#include <nova/serial/serial.h>

using namespace Nova;



extern "C" {
    ////////////////////////////////////////////////////////////
    /// @brief Prints the welcome message
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE static void PrintLogo(void)
    {
        Print("------------------------------------------------\n");
        Print(" __  _  ____ __  __ ____     ____   ____        \n");
        Print("|  \\| |/ () \\\\ \\/ // () \\   / () \\ (_ (_` \n");
        Print("|_|\\__|\\____/ \\__//__/\\__\\  \\____/.__)__) \n");
        Print("------------------------------------------------\n");
    }

    ////////////////////////////////////////////////////////////
    /// @brief Prints the compiler information
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE static void PrintCompilerInfo(void)
    {
        Log("Compiled %s by %s (arch %s)", NOVA_BUILD_DATETIME, NOVA_COMPILER, NOVA_ARCHITECTURE);
    }

    ////////////////////////////////////////////////////////////
    /// @brief Kernel entry point
    ///
    /// @param boot_handover_eax The value of the EAX register 
    ///                          at the time of the boot handover
    ///
    ////////////////////////////////////////////////////////////
    void kmain(u32 boot_handover_eax) 
    {
        InitializeTerminal();

        PrintLogo();
        PrintCompilerInfo();

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