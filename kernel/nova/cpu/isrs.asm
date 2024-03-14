; isrs.asm
; Defines the interrupt service routines for the CPU exceptions and IRQs
extern on_exception_interrupt
extern on_irq_interrupt


handle_exception_interrupt:
    ; 1. Save state of CPU by saving all registers
    ; 2. Switching to kernel data segment (store old one)
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Push the error code and the interrupt index that is further down the stack to the top of the stack
    push dword [esp + 36]
    push dword [esp + 44]

    ; Call the IRQ handler (defined in C)
    call on_exception_interrupt

    ; Pop the error code and the interrupt index from the stack
    pop eax
    pop eax

    ; 1. Restore CPU state by restoring all registers and switching back to the data segment used before the interrupt
    ; 2. Cleans up the stack by popping the error code and the return address from the stack
    ; 3. Enables interrupts again
    ; 4. Jumps back to the instruction that caused the interrupt (iret)
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    add esp, 8
    sti
    iret

handle_irq_interrupt:
    ; 1. Save state of CPU by saving all registers
    ; 2. Switching to kernel data segment (store old one)
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Push the IRQ number and IRS index that is further down the stack to the top of the stack
    push dword [esp + 36]
    push dword [esp + 44]

    ; Call the IRQ handler (defined in C)
    call on_irq_interrupt

    ; Pop the IRQ number and the IRS index from the stack
    pop ebx
    pop ebx

    ; 1. Restore CPU state by restoring all registers and switching back to the data segment used before the interrupt
    ; 2. Cleans up the stack by popping the error code and the return address from the stack
    ; 3. Enables interrupts again
    ; 4. Jumps back to the instruction that caused the interrupt (iret)
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx
    popa
    add esp, 8
    sti
    iret


; For some of the CPU exceptions, the error code is pushed onto the stack by the CPU automatically
; For all other exceptions, we push a dummy error code of 0 onto the stack to keep a consistent stack layout

%macro cpuex_nocode 1
    global cpuex_%1

    cpuex_%1:
        cli
        push byte 0
        push byte %1
        jmp handle_exception_interrupt
%endmacro

%macro cpuex_ercode 1
    global cpuex_%1

    cpuex_%1:
        cli
        push byte %1
        jmp handle_exception_interrupt
%endmacro

%macro irq_handler 2
    global irq_%1

    irq_%1:
        cli
        push byte %1
        push byte %2
        jmp handle_irq_interrupt
%endmacro

cpuex_nocode 0; Exception 0: Divide by zero 
cpuex_nocode 1; Exception 1: Debug
cpuex_nocode 2; Exception 2: Non-maskable interrupt
cpuex_nocode 3; Exception 3: Breakpoint
cpuex_nocode 4; Exception 4: 'Into detected overflow'
cpuex_nocode 5; Exception 5: Out of bounds
cpuex_nocode 6; Exception 6: Invalid opcode
cpuex_nocode 7; Exception 7: No coprocessor
cpuex_ercode 8; Exception 8: Double fault
cpuex_nocode 9; Exception 9: Coprocessor segment overrun
cpuex_ercode 10; Exception 10: Bad TSS
cpuex_ercode 11; Exception 11: Segment not present
cpuex_ercode 12; Exception 12: Stack fault
cpuex_ercode 13; Exception 13: General protection fault
cpuex_ercode 14; Exception 14: Page fault
cpuex_nocode 15; Exception 15: Reserved
cpuex_nocode 16; Exception 16: x87 FPU floating-point error
cpuex_ercode 17; Exception 17: Alignment check
cpuex_nocode 18; Exception 18: Machine check
cpuex_nocode 19; Exception 19: SIMD floating-point exception
cpuex_nocode 20; Exception 20: Virtualization exception
cpuex_ercode 21; Exception 21: Control Protection
cpuex_nocode 22; Exception 22: Reserved
cpuex_nocode 23; Exception 23: Reserved
cpuex_nocode 24; Exception 24: Reserved
cpuex_nocode 25; Exception 25: Reserved
cpuex_nocode 26; Exception 26: Reserved
cpuex_nocode 27; Exception 27: Reserved
cpuex_nocode 28; Exception 28: Hypervisor injection exception
cpuex_ercode 29; Exception 29: VMM communication exception
cpuex_ercode 30; Exception 30: Security exception
cpuex_nocode 31; Exception 31: Reserved

irq_handler 0, 32; IRQ 0: System timer
irq_handler 1, 33; IRQ 1: Keyboard
irq_handler 2, 34; IRQ 2: Cascade (used internally by the two PICs. never raised)
irq_handler 3, 35; IRQ 3: COM2
irq_handler 4, 36; IRQ 4: COM1
irq_handler 5, 37; IRQ 5: LPT2
irq_handler 6, 38; IRQ 6: Floppy disk
irq_handler 7, 39; IRQ 7: LPT1 / Unreliable "spurious" interrupt (usually)
irq_handler 8, 40; IRQ 8: CMOS real-time clock (if enabled)

