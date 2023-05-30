/**
 * spinlock.cpp
 * Locks that are used to protect critical sections of code from being executed
 * at the same time by multiple threads. Includes both reentrant and non-reentrant locking
*/
#include <simux/spinlock.h>
#include <simux/cpu/flgreg.h>
#include <simux/cpu/irq.h>


void spinlock_aqquire_irq_save(spinlock_t& lock, unsigned long& flags)
{
    flags = flagreg_dump();
    irq_disable_interrupts();

    spinlock_aqquire(lock);
}

void spinlock_release_irq_save(spinlock_t& lock, const unsigned long& flags)
{
    spinlock_release(lock);

    if (flagreg_dump_check_bit(flags))
    {
        irq_enable_interrupts();
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

