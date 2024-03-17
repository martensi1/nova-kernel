/**
 * exceptions.cpp
 * Responsible CPU exception catching and handling
*/
#include <nova/cpu/exceptions.h>
#include <nova/cpu/idt.h>
#include <nova/kernel.h>

using namespace Nova;


#define LOCATE_ISR(x) extern "C" void cpuex_##x(void);

LOCATE_ISR(0);
LOCATE_ISR(1);
LOCATE_ISR(2);
LOCATE_ISR(3);
LOCATE_ISR(4);
LOCATE_ISR(5);
LOCATE_ISR(6);
LOCATE_ISR(7);
LOCATE_ISR(8);
LOCATE_ISR(9);
LOCATE_ISR(10);
LOCATE_ISR(11);
LOCATE_ISR(12);
LOCATE_ISR(13);
LOCATE_ISR(14);
LOCATE_ISR(15);
LOCATE_ISR(16);
LOCATE_ISR(17);
LOCATE_ISR(18);
LOCATE_ISR(19);
LOCATE_ISR(20);
LOCATE_ISR(21);
LOCATE_ISR(22);
LOCATE_ISR(23);
LOCATE_ISR(24);
LOCATE_ISR(25);
LOCATE_ISR(26);
LOCATE_ISR(27);
LOCATE_ISR(28);
LOCATE_ISR(29);
LOCATE_ISR(30);
LOCATE_ISR(31);


/// @brief Sets up the IDT gates for the exceptions (CPU interrupts)
void exceptions_setup_gates(void)
{
    #define SETUP_GATE(x) idt_setup_gate(IDT_EXCEPTIONS_START + x, (u32)cpuex_##x, IDT_TRAP_GATE_PL0)

    SETUP_GATE(0);
    SETUP_GATE(1);
    SETUP_GATE(2);
    SETUP_GATE(3);
    SETUP_GATE(4);
    SETUP_GATE(5);
    SETUP_GATE(6);
    SETUP_GATE(7);
    SETUP_GATE(8);
    SETUP_GATE(9);
    SETUP_GATE(10);
    SETUP_GATE(11);
    SETUP_GATE(12);
    SETUP_GATE(13);
    SETUP_GATE(14);
    SETUP_GATE(15);
    SETUP_GATE(16);
    SETUP_GATE(17);
    SETUP_GATE(18);
    SETUP_GATE(19);
    SETUP_GATE(20);
    SETUP_GATE(21);
    SETUP_GATE(22);
    SETUP_GATE(23);
    SETUP_GATE(24);
    SETUP_GATE(25);
    SETUP_GATE(26);
    SETUP_GATE(27);
    SETUP_GATE(28);
    SETUP_GATE(29);
    SETUP_GATE(30);
    SETUP_GATE(31);
}

// Called from assembly ISRs
extern "C" void on_exception_interrupt(u32 error_code, u32 interrupt_index)
{
    static_cast<void>(error_code);
    EnterPanic("CPU exception", interrupt_index);
}