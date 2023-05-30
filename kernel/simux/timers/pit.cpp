/**
 * pit.cpp
 * Defines the Programmable Interval Timer (PIT) interface
*/
#include <simux/pic/pic.h>
#include <simux/cpu/sysbus.h>



#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_CHANNEL1_DATA_PORT 0x41
#define PIT_CHANNEL2_DATA_PORT 0x42
#define PIT_COMMAND_PORT 0x43



#define PIT_CHANNEL(x) (x << 6)



#define PRIMARY_PIC_COMMAND_PORT 0x20
#define PRIMARY_PIC_DATA_PORT 0x21

#define SECONDARY_PIC_COMMAND_PORT 0xA0
#define SECONDARY_PIC_DATA_PORT 0xA1


#define ICW1_INIT 0x10 // Initialization
#define ICW1_ICW4 0x01 // If ICW4 is needed (1) or not (0)
#define ICW4_8086 0x01 // Microprocessor mode (8086/8088: 1, MCS-80/85: 0)

#define PIC_COMMAND_EOI 0x20 // End of Interrupt


