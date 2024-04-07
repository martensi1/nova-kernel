#include <nova/print.h>
#include <nova/tty.h>
#include <nova/build.h>

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
    int kmain(uint32_t boot_handover_eax, uint32_t boot_handover_ebx)
    {
        static_cast<void>(boot_handover_ebx);
        static_cast<void>(boot_handover_eax);

        initialize_terminal();

        print_logo();
        print_compiler_info();

        log("Hello world!");
        return 0;
    }
}