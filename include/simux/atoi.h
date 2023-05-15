#ifndef __SIMUX_ATOI_H__
#define __SIMUX_ATOI_H__


extern "C" {
    int atoi(const char* str);
    char* itoa(int value, char* str, int base);
}


#endif // __SIMUX_ATOI_H__