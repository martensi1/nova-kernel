global entrypoint

extern kmain
extern kernel_start, ctors_start_addr, ctors_end_addr, dtors_start_addr, dtors_end_addr


section .multiboot
    ; The executable must start with a multiboot header to be loaded by GRUB
    ; See specification (https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
    %define MULTIBOOT_HEADER_MAGIC 0x1BADB002
    %define MULTIBOOT_HEADER_FLAGS 0x00000003
    %define MULTIBOOT_HEADER_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    align 4
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_HEADER_CHECKSUM


section .bss
    ; Define a stack for the kernel (16 KiB)
    %define STACK_SIZE 0x1000
    align 16

    stack:
        resb STACK_SIZE

section .data
    boot_handover_ebx dd 0
    boot_handover_eax dd 0


section .text
    entrypoint:
        ; The boot handover information is passed by GRUB in the registers
        mov [boot_handover_ebx], ebx
        mov [boot_handover_eax], eax

        ; Setup the stack
        mov esp, stack + STACK_SIZE

        ; Run C++ global constructors
        call run_ctors

        ; Call the kernel's main function
        mov ebx, [boot_handover_ebx]
        mov eax, [boot_handover_eax]
        push ebx
        push eax

        call kmain
        add esp, 8

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
