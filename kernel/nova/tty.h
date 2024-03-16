#ifndef __NOVA_TERMINAL_H__
#define __NOVA_TERMINAL_H__

#include <nova/common.h>


void term_initialize();
void term_clear();

void term_write(const char* data, size_t size);
void term_write_str(const char* str);


#endif // __NOVA_TERMINAL_H__