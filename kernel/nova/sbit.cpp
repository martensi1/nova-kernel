/**
 * sbit.cpp
 * SBIT (Startup Built-in Test) is a test that checks if the kernel has started correctly and 
 * set up all the vital components.
*/
#include <nova/sbit.h>
#include <nova/kernel.h>
#include <nova/locks.h>
#include <nova/cpu/irq.h>


#define TEST_OK(description) logk(" [OK]: %s", description)
#define TEST_FAIL(description, data) kpanic(description, data)

static SpinLock irq0_counter_lock = SpinLock();
static volatile uint8_t irq0_counter = 0;


static void on_irq0()
{
    SpinGuard guard(irq0_counter_lock);
    irq0_counter++;
}


static void check_timer()
{
    irq_handler_t old_handler = irq_remove_handler(IRQ0);
    irq_add_handler(IRQ0, (irq_handler_t)on_irq0);

    bool is_ticking = false;

    for (int i = 0; i < 100; i++) {
        asm volatile("hlt");
        SpinGuard guard(irq0_counter_lock);
        
        if (irq0_counter > 10) {
            is_ticking = true;
            irq0_counter_lock.release();
            break;
        }
    }

    static_cast<void>(irq_remove_handler(IRQ0));
    static_cast<void>(old_handler);
    // irq_add_handler(IRQ0, old_handler);

    if (is_ticking)
    {
        TEST_OK("IRQ0 is ticking");
    }
    else
    {
        TEST_FAIL("IRQ0 not ticking", irq0_counter);
    }
}


/// @brief Runs the SBIT (Startup Built-in Test)
void sbit_run()
{
    logk("Running SBIT (Startup Buildt-in Test)");

    check_timer();

    logk("SBIT passed!");
}