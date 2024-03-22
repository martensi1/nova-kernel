export CC := g++
export AS := nasm

export ASM_FLAGS :=
export CPP_FLAGS := -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-use-cxa-atexit -fno-rtti -Wall -Wextra -Werror -std=c++11
export LD_FLAGS = -z noexecstack


.PHONY: loader test

loader:
	make -C ./boot/ all

test:
	make -C ./boot/ test