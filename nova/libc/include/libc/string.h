#ifndef __NOVA_LIBC_STRING_H__
#define __NOVA_LIBC_STRING_H__

#include <stddef.h>

void* memcpy(void *dst, const void* src, int n);
void* memset(void* ptr, int value, size_t num);

char* strcat(char* destination, const char* source);

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);

char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t num);

size_t strlen(const char* str);


#endif // __NOVA_LIBC_STRING_H__