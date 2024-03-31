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
    /// @brief Prints the welcome message
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE static void print_logo(void)
    {
        print("------------------------------------------------\n");
        print(" __  _  ____ __  __ ____     ____   ____        \n");
        print("|  \\| |/ () \\\\ \\/ // () \\   / () \\ (_ (_` \n");
        print("|_|\\__|\\____/ \\__//__/\\__\\  \\____/.__)__) \n");
        print("------------------------------------------------\n");
    }

    ////////////////////////////////////////////////////////////
    /// @brief Prints the compiler information
    ///
    ////////////////////////////////////////////////////////////
    FORCE_INLINE static void print_compiler_info(void)
    {
        log("Compiled %s by %s (arch %s)", NOVA_BUILD_DATETIME, NOVA_COMPILER, NOVA_ARCHITECTURE);
    }

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

        print_logo();
        print_compiler_info();

        run_hbit(boot_handover_eax);

        u16 gdt_size;
        setup_gdt(0x800, gdt_size);

        load_kernel(boot_handover_ebx);
    }
}