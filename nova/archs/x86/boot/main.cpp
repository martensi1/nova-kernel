#include <nova/tty.h>
#include <nova/print.h>
#include <nova/build.h>
#include <nova/macros.h>
#include <arch/cpu/gdt.h>
#include <arch/serial/serial.h>

#include "hbit.h"
#include "loader.h"


extern "C" {
    ////////////////////////////////////////////////////////////
    /// @brief Prints the compiler information
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE static void print_compiler_info(void)
    {
        log("Compiled %s by %s (arch %s)", NOVA_BUILD_DATETIME, NOVA_COMPILER, NOVA_ARCHITECTURE);
    }

    ////////////////////////////////////////////////////////////
    /// @brief Loader main function
    ///
    /// @param boot_handover_eax The value of the EAX register 
    ///                          at the time of the boot handover
    /// @param boot_handover_ebx The value of the EBX register
    ///                          at the time of the boot handover
    ///
    ////////////////////////////////////////////////////////////
    void kernel_loader_main(u32 boot_handover_eax, u32 boot_handover_ebx) 
    {
        initialize_terminal();
        
        print_compiler_info();
        run_hbit(boot_handover_eax);

        Serial::scanForPorts();
        terminal_scan_drivers();

        u16 gdt_size;
        setup_gdt(0x800, gdt_size);

        load_kernel(boot_handover_ebx);
    }
}