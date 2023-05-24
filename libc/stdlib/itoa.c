#include <libc/stdlib.h>
#include <stdbool.h>
#undef itoa


char* itoa(int value, char * buffer, int base)
{
    char* ptr = buffer;
    bool negative = false;

    if (value == 0) {
        *ptr++ = '0';
        *ptr++ = '\0';

        return buffer;
    }
 
    // Negative numbers only handled for base 10
    if (value < 0 && base == 10) {
        negative = true;
        value = -value;
    }
 
    // Process individual digits (from right to left)
    // Use remainder to get the digits for each position
    while (value != 0) {
        int remainder = value % base;

        char c = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        *ptr++ = c;

        value = value / base;
    }
 
    if (negative)
        *ptr++ = '-';
 
    *ptr = '\0';
 
    // Done!
    // Reverse the string to get the correct order
    ptr--;
    char* start = buffer;

    while (start < ptr) {
        char temp = *start;
        *start++ = *ptr;
        *ptr-- = temp;
    }

    return buffer;
}
