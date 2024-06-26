#include <nova/tty.h>
#include <nova/kernel.h>
#include <arch/pit.h>
#include <nova/build.h>
#include <arch/serial/serial.h>
#include <arch/cpu/setup.h>
#include <arch/sbit.h>

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
    ////////////////////////////////////////////////////////////
    int kmain_32()
    {
        initialize_terminal();

        print_logo();
        print_compiler_info();

        Serial::scanForPorts();
        terminal_scan_drivers();

        cpu_setup();

        SetupPIT(200);
        run_sbit();

        return 0;
    }
}