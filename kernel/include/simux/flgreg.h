#ifndef __SIMUX_FLAGS_REGISTRY_H__
#define __SIMUX_FLAGS_REGISTRY_H__

#include <stdint.h>


enum cpu_flag {
	CPUFLAG_ID = 21,
};


/// @brief Tests if specified CPU flag is changeable
/// @param flag Flag to test
/// @return True if flag is changeable, false otherwise
inline bool flagreg_test_if_changeable(cpu_flag flag)
{
	uint32_t flags_1 = 0;
    uint32_t flags_2 = 0;

    uint32_t mask = 1 << (uint32_t)flag;

    // 1. Push CPU flags onto the stack
    // 2. Pop flags to flags_1, also copy to flags_2
	// 3. Flip specified flag in flags_1
	// 4. Push flags_1 onto the stack and pop from stack to CPU flags
	// 5. Push CPU flags onto the stack again
	// 6. Pop flags to flags_1
	// 7. Compare flags_1 and flags_2, if they are different, then the flag is changeable  
	asm volatile (
		"pushfl		\n\t"
		"popl %0		\n\t"
		"movl %0, %1	\n\t"
		"xorl %2, %0	\n\t"
		"pushl %0		\n\t"
		"popfl		\n\t"
		"pushfl		\n\t"
		"popl %0		\n\t"
		: "=&r" (flags_1), "=&r" (flags_2)
		: "ir" (mask)
    );

	uint32_t nonzero_if_different = flags_1 ^ flags_2;
	return nonzero_if_different != 0;
}


#endif // __SIMUX_FLAGS_REGISTRY_H__