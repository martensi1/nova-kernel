# Nova

Nova is a hobby operating system project. The goal is to create a simple, yet functional, operating system from scratch. The project is written in C++ and currently only supports x86-family architectures.

## Structure

- `docs` - Documentation
- `nova` - Kernel source code
- `sysroot` - The initial file system of the OS
- `qemu` - Emulation scripts

## Build

Install the following dependencies:

- `build-essential`:
- `make`
- `nasm`
- `grub2`
- `qemu-system`
- `fdisk`
- `gdb`

## Emulate OS

To test out the OS, run `make test-32` in the root of the repository. This will:

1. Build the kernel
2. Create a bootable image with GRUB and the kernel
3. Launch the bootable image in QEMU

## Author

Simon Alm-Mårtensson
