# Simux

Hobby project OS written in C++ and C

# Architectures

Currently supports:

- x86-family

# Structure

- `kernel` - Kernel source code
- `libc` - libc implementation used by kernel
- `sysroot` - the initial file system of the OS

## Development

1. Install Vagrant and VirtualBox
2. Clone repository and run `vagrant up --provision` to setup a new development environment
3. Run `vagrant ssh` to SSH into the VM instance

## Author

Simon Mårtensson
