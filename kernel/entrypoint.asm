global entrypoint

extern kmain
extern ctors_start_addr, ctors_end_addr, dtors_start_addr, dtors_end_addr



section .multiboot

# The executable must start with a multiboot header to be loaded by GRUB
# See specification (https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
%define MULTIBOOT_HEADER_MAGIC 0x1BADB002
%define MULTIBOOT_HEADER_FLAGS 0x00000003
%define MULTIBOOT_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

align 4
dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd MULTIBOOT_CHECKSUM



section .bss

# Define a stack for the kernel (16 KiB)
%define STACK_SIZE 0x1000
align 16

stack:
    resb STACK_SIZE



section .text

# Run C++ global constructors before main
run_ctors:
    mov ebx, ctors_start_addr
    jmp .next
.run:
    call [ebx]
    add ebx, 4
.next:
    cmp ebx, ctors_end_addr
    jp .run

# Main entrypoint, run kernel main function!
entrypoint:
    push eax
    push ebx

    mov esp, stack + STACK_SIZE
    call kmain

# Run C++ global destructors after main
run_dtors:
    mov ebx, dtors_start_addr
    jmp .next
.run:
    call [ebx]
    add ebx, 4
.next:
    cmp ebx, dtors_end_addr
    jp .run

# Halt the CPU (go to sleep)
cli
hlt
jmp 1b
