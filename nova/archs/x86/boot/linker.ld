/*
Define output format and entrypoint
*/
OUTPUT_FORMAT(elf32-i386) 
OUTPUT_ARCH(i386)
ENTRY(entrypoint)


/*
Define the memory layout of the program and the ELF sections
The sections start at an offset of 0x100000 (1MB) in RAM and are aligned to 0x1000 (4KB) boundaries
The reason for this is to avoid conflicts with the bootloader and other system components that may be loaded at lower memory addresses
*/
SECTIONS {
    . = 0x100000;
    kernel_start = .;

    .text : {
        *(.multiboot)
        *(.text)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    /*
    Arrange global constructors and destructors and specify the start and end addresses
    The start and end addresses are used in the assembly file to call the constructors and destructors before
    and after the main function
    */
    .init_array : {
        ctors_start_addr = .;
        KEEP (*(.init_array))
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        ctors_end_addr = .;
    }
    .fini_array : {
        dtors_start_addr = .;
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
        KEEP (*(.fini_array))
        dtors_end_addr = .;
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .bss ALIGN (0x1000) : {
        *(.bss)
    }
}