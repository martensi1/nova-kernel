# Simux

Hobby project OS written in C++ and C

# Architectures

Currently supports:

- x86-family

# Structure

- `kernel` - Kernel source code
- `libc` - libc implementation used by kernel
- `sysroot` - the initial file system of the OS

## Setup

1. Install Vagrant and VirtualBox
2. Clone repository. Be sure that the checked out files have unix-style line endings, `LF`)
   Example: `git clone --config core.autocrlf=false --config core.eol lf <repo-url>`
3. Run `vagrant up --provision` to setup a new development environment
4. Run `vagrant ssh` to SSH into the VM instance

## Run OS

To test out the OS, run `make test` in the root of the repository. This will:

1. Build the kernel (along with libc)
2. Create a bootable image with GRUB and the kernel
3. Launch the bootable image in QEMU

## Author

Simon Mårtensson
