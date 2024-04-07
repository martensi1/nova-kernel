#include <nova/tty.h>
#include <nova/print.h>
#include <nova/build.h>
#include <nova/macros.h>

#include <arch/gdt.h>

#include "hbit.h"
#include "loader.h"

using namespace nova;



extern "C" {
    ////////////////////////////////////////////////////////////
    /// @brief Kernel entry point (32-bit)
    ///
    /// @param boot_handover_eax The value of the EAX register 
    ///                          at the time of the boot handover
    /// @param boot_handover_ebx The value of the EBX register
    ///                          at the time of the boot handover
    ///
    ////////////////////////////////////////////////////////////
    void kernel_entry_point(u32 boot_handover_eax, u32 boot_handover_ebx) 
    {
        static_cast<void>(boot_handover_ebx);

        initialize_terminal();

        run_hbit(boot_handover_eax);

        u16 gdt_size;
        setup_gdt(0x800, gdt_size);

        load_kernel(boot_handover_ebx);
    }
}