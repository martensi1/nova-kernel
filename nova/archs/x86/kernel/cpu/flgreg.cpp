/**
 * flgreg.cpp
 * Functionality for interacting with the CPU's FLAGS registry
*/
#include <arch/cpu/flgreg.h>


/// @brief Tests if specified CPU flag is changeable
/// @param flag Flag to test
/// @return True if flag is changeable, false otherwise
bool flagreg_test_if_changeable(const cpu_flag_t flag)
{
	u32 flags_1 = 0;
    u32 flags_2 = 0;

    u32 mask = 1 << (u32)flag;

    // 1. Push CPU flags onto the stack
    // 2. Pop flags to flags_1, also copy to flags_2
	// 3. Flip specified flag in flags_1
	// 4. Push flags_1 onto the stack and pop from stack to CPU flags
	// 5. Push CPU flags onto the stack again
	// 6. Pop flags to flags_1
	// 7. Compare flags_1 and flags_2, if they are different, then the flag is changeable  
	asm volatile (
		"pushfl		 \n"
		"popl %0     \n"
		"movl %0, %1 \n"
		"xorl %2, %0 \n"
		"pushl %0	 \n"
		"popfl		 \n"
		"pushfl		 \n"
		"popl %0	 \n"
		: "=&r" (flags_1), "=&r" (flags_2)
		: "ir" (mask)
    );

	u32 nonzero_if_different = flags_1 ^ flags_2;
	return nonzero_if_different != 0;
}