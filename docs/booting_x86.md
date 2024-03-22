# Booting

This document describes the boot process of Nova on an x86 system. 

## Boot Loader

The boot loader is a small program that is responsible for loading the operating system into memory and starting its execution. The boot loader is typically stored in the first sector of the disk, known as the Master Boot Record (MBR). The BIOS loads the boot loader into memory and transfers control to it. The boot loader then loads the operating system into memory and transfers control to it.

Nova uses the GRUB boot loader which follows the Multiboot Specification. The Multiboot Specification is a standard that defines how the boot loader should load the operating system into memory and transfer control to it.

Specification: [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)

The boot loader will load N

### Memory Layout

The memory layout of an x86 system is as follows:
```
+------------------+ <- 0xFFFFFFFF (4GB)
|     ...          |
|                  |
|                  |
|                  |
+------------------+ <- Depends
|  Protected-Mode  |
|  Kernel (32-bit) |
+------------------+ <- 0x00100000 (1MB)
|  Video/IO memory |
|    Boot Loader   |
|       BIOS       |
+------------------+ <- 0x00000000 (0MB)
```

### Boot Process (32-bit)




### Boot Process (64-bit)
