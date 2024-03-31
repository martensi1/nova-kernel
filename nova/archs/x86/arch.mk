include archs/x86/boot/module.mk
include archs/x86/kernel/module.mk


ifeq ($(ARCH), i386)
	CXXFLAGS += -m32 -DNOVA_ARCH_I386=1
	ASFLAGS += -f elf32
	LDFLAGS += -m elf_i386
else
	CXXFLAGS += -m32 -DNOVA_ARCH_X86_64=1
	ASFLAGS += -f elf32
	LDFLAGS += -m elf_i386
endif