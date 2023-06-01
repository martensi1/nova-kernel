/**
 * spinlock.cpp
 * Spinlocks are small and simple locks with minimal overhead. They are used to protect critical
 * sections of code from being executed at the same time by multiple threads. Includes both
 * non-reentrant and reentrant locking (interrupt safe)
*/
#include <simux/spinlock.h>
#include <simux/cpu/flgreg.h>
#include <simux/cpu/irq.h>


void spinlock_aqquire_irq_save(spinlock_t& lock, unsigned long& flags)
{
    flags = flagreg_dump();
    irq_disable();

    spinlock_aqquire(lock);
}

void spinlock_release_irq_restore(spinlock_t& lock, const unsigned long& flags)
{
    spinlock_release(lock);

    if (flagreg_dump_check_bit(flags))
    {
        irq_enable();
    }
}

void spinlock_aqquire(spinlock_t& lock)
{
    while (__atomic_test_and_set(&lock, __ATOMIC_ACQUIRE))
    {
        __builtin_ia32_pause();
    }
}

void spinlock_release(spinlock_t& lock)
{
    __atomic_clear(&lock, __ATOMIC_RELEASE);
}

