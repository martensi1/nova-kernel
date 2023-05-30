/**
 * flgreg.h
 * Functionality for interacting with the CPU's FLAGS registry
*/
#ifndef __SIMUX_FLAGS_REGISTRY_H__
#define __SIMUX_FLAGS_REGISTRY_H__

#include <simux/types.h>


/// @brief List of FLAGS registry flags
typedef enum {
	CPUFLAG_IF = 9,  // Interrupt enable flag
	CPUFLAG_ID = 21, // CPUID instruction available flag
} cpu_flag_t;


/// @brief Dumps all CPU flags to a long
/// @return Long containing all CPU flags (bitfield)
inline unsigned long flagreg_dump(void)
{
	long flags = 0;
	asm volatile ("pushfl; popl %0" : "=r" (flags));
	return flags;
}


/// @brief Checks if specified CPU flag is set (from a dump)
/// @param flags Dumped CPU flags
/// @return True if flag is set, false otherwise
inline bool flagreg_dump_check_bit(const unsigned long& flags)
{
	return flags & (1 << (u32)CPUFLAG_IF);
}


/// @brief Tests if specified CPU flag is changeable
/// @param flag Flag to test
/// @return True if flag is changeable, false otherwise
bool flagreg_test_if_changeable(const cpu_flag_t flag);


#endif // __SIMUX_FLAGS_REGISTRY_H__