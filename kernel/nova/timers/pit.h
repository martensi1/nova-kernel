/**
 * pit.h
 * Defines the Programmable Interval Timer (PIT) interface
*/
#ifndef __NOVA_PIT_H__
#define __NOVA_PIT_H__

#include <nova/common.h>


void pic_setup_interrupt_generator(const u32 frequency);


#endif // __NOVA_PIT_H__