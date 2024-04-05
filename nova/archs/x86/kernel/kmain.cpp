#include <nova/print.h>

extern "C" {
    int kmain()
    {
        log("Hello world!");
        return 0;
    }
}