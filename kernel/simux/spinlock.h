/**
 * spinlock.h
 * Spinlocks are small and simple locks with minimal overhead. They are used to protect critical
 * sections of code from being executed at the same time by multiple threads. Includes both
 * non-reentrant and reentrant locking (interrupt safe)
*/
#ifndef __SIMUX_SPINLOCK_H__
#define __SIMUX_SPINLOCK_H__

#include <simux/types.h>

typedef u32 spinlock_t;
#define SPINLOCK_UNLOCKED 0


#define spin_lock_irqsave(lock) unsigned long __cpu_flags; spinlock_aqquire_irq_save(lock, __cpu_flags)
#define spin_unlock_irqrestore(lock) spinlock_release_irq_save(lock, __cpu_flags)


void spinlock_aqquire_irq_save(spinlock_t& lock, unsigned long& flags);
void spinlock_release_irq_save(spinlock_t& lock, const unsigned long& flags);

void spinlock_aqquire(spinlock_t& lock);
void spinlock_release(spinlock_t& lock);


#endif // __SIMUX_SPINLOCK_H__