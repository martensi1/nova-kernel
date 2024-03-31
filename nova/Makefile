#########################################
# Base variables
#########################################
CXX := g++
AS := nasm
MV := mv -f
RM := rm -f
SED := sed


#########################################
# Architecture
#########################################
ifeq ($(ARCH), i386)
	ARCH_DIR := x86
else ifeq ($(ARCH), x86_64)
	ARCH_DIR += x86
else
$(error Target architecture not defined or not supported!)
endif

ARCH_DIR := archs/$(ARCH_DIR)


#########################################
# Base flags
#########################################
CXXFLAGS := -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs \
-fno-use-cxa-atexit -fno-rtti -Wall -Wextra -Werror -std=c++11

LDFLAGS := -z noexecstack

ASFLAGS :=


#########################################
# Include directories
#########################################
NOVA_INCLUDE := $(CURDIR)/include
LIBC_INCLUDE := $(CURDIR)/libc/include
ARCH_INCLUDE := $(CURDIR)/$(ARCH_DIR)/include

INCLUDE_DIRS := $(NOVA_INCLUDE) $(LIBC_INCLUDE) $(ARCH_INCLUDE)
CXXFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))

NOVA_SRC := $(CURDIR)/kernel
ARCH_SRC := $(CURDIR)/$(ARCH_DIR)/kernel


#########################################
# Modules
#########################################
executables :=
libraries :=
sources :=

source-to-object = $(subst .cpp,.o,$(filter %.cpp,$1)) \
                   $(subst .asm,.o,$(filter %.asm,$1)) \
				   $(subst .c,.o,$(filter %.c,$1))

subdirectory = $(patsubst %/module.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

define make-executable
	executables += $2
	sources += $1

$2: $(call source-to-object,$1)
	$(LD)  $(LD_FLAGS) -o $$@ $$^ ../../../libc/bin/libc.a $3
endef

define make-library
	libraries += $2
	sources += $1

$2: $(call source-to-object,$1)
	$(LD)  $(LD_FLAGS) -o $$@ $$^ -r $3
endef

include $(ARCH_DIR)/arch.mk
include libc/module.mk

objects = $(call source-to-object,$(sources))


#########################################
# Targets
#########################################
all: $(executables) $(libraries)

%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -DNOVA_ARCH_I386=1 -c $< -o $@

%.o: %.asm
	$(AS) $(ASM_FLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(objects) $(programs)
