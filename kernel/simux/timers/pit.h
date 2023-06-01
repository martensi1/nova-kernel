/**
 * pit.h
 * Defines the Programmable Interval Timer (PIT) interface
*/
#ifndef __SIMUX_PIT_H__
#define __SIMUX_PIT_H__

#include <simux/types.h>


void pic_setup_interrupt_generator(const u32 frequency);


#endif // __SIMUX_PIT_H__