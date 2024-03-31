local_src := 
local_src += $(wildcard $(subdirectory)/*.cpp)
local_src += $(wildcard $(subdirectory)/**/*.cpp)
local_src += $(wildcard $(subdirectory)/*.asm)

local_src += $(wildcard $(NOVA_SRC)/*.cpp)
local_src += $(wildcard $(NOVA_SRC)/**/*.cpp)

$(eval $(call make-executable, $(local_src), $(subdirectory)/kernel.elf, -T $(subdirectory)/linker.ld))