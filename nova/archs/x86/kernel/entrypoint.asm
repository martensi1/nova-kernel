global entrypoint

extern kmain_32
extern kernel_start, ctors_start_addr, ctors_end_addr, dtors_start_addr, dtors_end_addr


section .bss
    ; Define a stack for the kernel (16 KiB)
    %define STACK_SIZE 0x1000
    align 16

    stack:
        resb STACK_SIZE

section .data
    multiboot_info_addr dd 0


section .text
    entrypoint:
        ; The multiboot information is passed in the eax register
        mov [multiboot_info_addr], eax

        ; Setup the stack
        mov esp, stack + STACK_SIZE

        ; Run C++ global constructors
        call run_ctors

        ; Call the kernel's main function
        mov eax, [multiboot_info_addr]
        push eax

        call kmain_32
        add esp, 4

        ; Run C++ global destructors
        call run_dtors

        ; Halt the CPU
        call halt

    run_ctors:
        mov ebx, ctors_start_addr
        jmp .next
        .run:
            call [ebx]
            add ebx, 4
        .next:
            cmp ebx, ctors_end_addr
            jne .run
            ret

    run_dtors:
        mov ebx, dtors_start_addr
        jmp .next
        .run:
            call [ebx]
            add ebx, 4
        .next:
            cmp ebx, dtors_end_addr
            jne .run
            ret

    halt:
        cli
        .loop:
            hlt
            jmp .loop
