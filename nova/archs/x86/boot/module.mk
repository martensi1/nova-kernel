local_src := 
local_src += $(wildcard $(subdirectory)/*.cpp)
local_src += $(wildcard $(subdirectory)/*.asm)

local_src += $(ARCH_SRC)/gdt.cpp
local_src += $(ARCH_SRC)/serial/serial_port.cpp
local_src += $(ARCH_SRC)/serial/serial.cpp
local_src += $(ARCH_SRC)/sync/spin_lock.cpp

local_src += $(CORE_SRC)/exec/elf32.cpp
local_src += $(CORE_SRC)/tty.cpp
local_src += $(CORE_SRC)/print.cpp
local_src += $(CORE_SRC)/panic.cpp
local_src += $(CORE_SRC)/drivers/serialcon.cpp
local_src += $(CORE_SRC)/drivers/vgacon.cpp

$(eval $(call make-executable, $(local_src), $(subdirectory)/loader.elf, -T $(subdirectory)/linker.ld, grub-start))