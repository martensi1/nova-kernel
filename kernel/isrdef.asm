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


global cpuex_0
global cpuex_1
global cpuex_2
global cpuex_3
global cpuex_4
global cpuex_5
global cpuex_6
global cpuex_7
global cpuex_8
global cpuex_9
global cpuex_10
global cpuex_11
global cpuex_12
global cpuex_13
global cpuex_14
global cpuex_15
global cpuex_16
global cpuex_17
global cpuex_18
global cpuex_19
global cpuex_20
global cpuex_21
global cpuex_22
global cpuex_23
global cpuex_24
global cpuex_25
global cpuex_26
global cpuex_27
global cpuex_28
global cpuex_29
global cpuex_30
global cpuex_31

; For some of the CPU exceptions, the error code is pushed onto the stack by the CPU automatically
; For all other exceptions, we push a dummy error code of 0 onto the stack to keep a consistent stack layout

; Exception 0: Divide by zero (no error code)
cpuex_0:
    cli
    push byte 0
    push byte 0
    jmp handle_exception_interrupt

; Exception 1: Debug (no error code)
cpuex_1:
    cli
    push byte 0
    push byte 1
    jmp handle_exception_interrupt

; Exception 2: Non-maskable interrupt (no error code)
cpuex_2:
    cli
    push byte 0
    push byte 2
    jmp handle_exception_interrupt

; Exception 3: Breakpoint (no error code)
cpuex_3:
    cli
    push byte 0
    push byte 3
    jmp handle_exception_interrupt

; Exception 4: 'Into detected overflow' (no error code)
cpuex_4:
    cli
    push byte 0
    push byte 4
    jmp handle_exception_interrupt

; Exception 5: Out of bounds (no error code)
cpuex_5:
    cli
    push byte 0
    push byte 5
    jmp handle_exception_interrupt

; Exception 6: Invalid opcode (no error code)
cpuex_6:
    cli
    push byte 0
    push byte 6
    jmp handle_exception_interrupt

; Exception 7: No coprocessor (no error code)
cpuex_7:
    cli
    push byte 0
    push byte 7
    jmp handle_exception_interrupt

; Exception 8: Double fault (error code)
cpuex_8:
    cli
    push byte 8
    jmp handle_exception_interrupt

; Exception 9: Coprocessor segment overrun (no error code)
cpuex_9:
    cli
    push byte 0
    push byte 9
    jmp handle_exception_interrupt

; Exception 10: Bad TSS (error code)
cpuex_10:
    cli
    push byte 10
    jmp handle_exception_interrupt

; Exception 11: Segment not present (error code)
cpuex_11:
    cli
    push byte 11
    jmp handle_exception_interrupt

; Exception 12: Stack fault (error code)
cpuex_12:
    cli
    push byte 12
    jmp handle_exception_interrupt

; Exception 13: General protection fault (error code)
cpuex_13:
    cli
    push byte 13
    jmp handle_exception_interrupt

; Exception 14: Page fault (error code)
cpuex_14:
    cli
    push byte 14
    jmp handle_exception_interrupt

; Exception 15: Reserved (no error code)
cpuex_15:
    cli
    push byte 0
    push byte 15
    jmp handle_exception_interrupt

; Exception 16: x87 FPU floating-point error (no error code)
cpuex_16:
    cli
    push byte 0
    push byte 16
    jmp handle_exception_interrupt

; Exception 17: Alignment check (error code)
cpuex_17:
    cli
    push byte 17
    jmp handle_exception_interrupt

; Exception 18: Machine check (no error code)
cpuex_18:
    cli
    push byte 0
    push byte 18
    jmp handle_exception_interrupt

; Exception 19: SIMD floating-point exception (no error code)
cpuex_19:
    cli
    push byte 0
    push byte 19
    jmp handle_exception_interrupt

; Exception 20: Virtualization exception (no error code)
cpuex_20:
    cli
    push byte 0
    push byte 20
    jmp handle_exception_interrupt

; Exception 21: Control Protection (error code)
cpuex_21:
    cli
    push byte 21
    jmp handle_exception_interrupt

; Exception 22: Reserved (no error code)
cpuex_22:
    cli
    push byte 0
    push byte 22
    jmp handle_exception_interrupt

; Exception 23: Reserved (no error code)
cpuex_23:
    cli
    push byte 0
    push byte 23
    jmp handle_exception_interrupt

; Exception 24: Reserved (no error code)
cpuex_24:
    cli
    push byte 0
    push byte 24
    jmp handle_exception_interrupt

; Exception 25: Reserved (no error code)
cpuex_25:
    cli
    push byte 0
    push byte 25
    jmp handle_exception_interrupt

; Exception 26: Reserved (no error code)
cpuex_26:
    cli
    push byte 0
    push byte 26
    jmp handle_exception_interrupt

; Exception 27: Reserved (no error code)
cpuex_27:
    cli
    push byte 0
    push byte 27
    jmp handle_exception_interrupt

; Exception 28: Hypervisor injection exception (no error code)
cpuex_28:
    cli
    push byte 0
    push byte 28
    jmp handle_exception_interrupt

; Exception 29: VMM communication exception (error code)
cpuex_29:
    cli
    push byte 29
    jmp handle_exception_interrupt

; Exception 30: Security exception (error code)
cpuex_30:
    cli
    push byte 30
    jmp handle_exception_interrupt

; Exception 31: Reserved (no error code)
cpuex_31:
    cli
    push byte 0
    push byte 31
    jmp handle_exception_interrupt


global irq_0
global irq_1
global irq_2
global irq_3
global irq_4
global irq_5
global irq_6
global irq_7
global irq_8

; IRQ 0: System timer
irq_0:
    cli
    push byte 0
    push byte 32
    jmp handle_irq_interrupt

; IRQ 1: Keyboard
irq_1:
    cli
    push byte 1
    push byte 33
    jmp handle_irq_interrupt

; IRQ 2: Cascade (used internally by the two PICs. never raised)
irq_2:
    cli
    push byte 2
    push byte 34
    jmp handle_irq_interrupt

; IRQ 3: COM2
irq_3:
    cli
    push byte 3
    push byte 35
    jmp handle_irq_interrupt

; IRQ 4: COM1
irq_4:
    cli
    push byte 4
    push byte 36
    jmp handle_irq_interrupt

; IRQ 5: LPT2
irq_5:
    cli
    push byte 5
    push byte 37
    jmp handle_irq_interrupt

; IRQ 6: Floppy disk
irq_6:
    cli
    push byte 6
    push byte 38
    jmp handle_irq_interrupt

; IRQ 7: LPT1 / Unreliable "spurious" interrupt (usually)
irq_7:
    cli
    push byte 7
    push byte 39
    jmp handle_irq_interrupt

; IRQ 8: CMOS real-time clock (if enabled)
irq_8:
    cli
    push byte 8
    push byte 40
    jmp handle_irq_interrupt

