programs :=
sources :=
libraries :=
extra_clean :=

MV := mv -f
RM := rm -f
SED := sed

CPP := g++
AS := nasm

CPP_FLAGS := -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-use-cxa-atexit -fno-rtti -Wall -Wextra -Werror -std=c++11 -m32
LD_FLAGS := -z noexecstack -m elf_i386
ASM_FLAGS := -f elf32

NOVA_INCLUDE := $(CURDIR)/../../include
LIBC_INCLUDE := $(CURDIR)/../../../libc/include
ARCH_INCLUDE := $(CURDIR)/include

NOVA_PATH := $(CURDIR)/../../kernel
ARCH_PATH := $(CURDIR)/kernel

INC_DIR := -I $(NOVA_INCLUDE) -I $(LIBC_INCLUDE) -I $(ARCH_INCLUDE)
CPP_FLAGS += $(INC_DIR)

source-to-object = $(subst .cpp,.o,$(filter %.cpp,$1)) \
                   $(subst .asm,.o,$(filter %.asm,$1)) \
				   $(subst .c,.o,$(filter %.c,$1))

subdirectory = $(patsubst %/module.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

define make-program
	programs += $1
	sources  += $3

$1: $(call source-to-object,$3)
	$(LD)  $(LD_FLAGS) -o $$@ $$^ ../../../libc/bin/libc.a -T $2
endef


include boot/module.mk
include kernel/module.mk



objects = $(call source-to-object,$(sources))

.PHONY: all
all: $(programs)


%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -DNOVA_ARCH_I386=1 -c $< -o $@
#	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
#	$(MV) $@.tmp $@

%.o: %.c
	$(CPP) $(CPP_FLAGS) -DNOVA_ARCH_I386=1 -c $< -o $@

%.o: %.asm
	$(AS) $(ASM_FLAGS) $< -o $@
#	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
#	$(MV) $@.tmp $@


clean:
	$(RM) $(objects) $(programs)

.PHONY: test
test:
	@echo $(programs)
	@echo $(sources)
	@echo $(libraries)
	@echo $(extra_clean)
	@echo $(subdirectory)
	@echo $(INC_DIR)
	@echo $(CPP_FLAGS)
	@echo $(call source-to-object,$(wildcard boot/*.cpp))