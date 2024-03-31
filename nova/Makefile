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
# Debug flags
#########################################
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
	LDFLAGS += -g
else
	CXXFLAGS += -O2
endif


#########################################
# Include directories
#########################################
THIS_DIR := .

NOVA_INCLUDE := $(THIS_DIR)/include
LIBC_INCLUDE := $(THIS_DIR)/libc/include
ARCH_INCLUDE := $(THIS_DIR)/$(ARCH_DIR)/include

INCLUDE_DIRS := $(NOVA_INCLUDE) $(LIBC_INCLUDE) $(ARCH_INCLUDE)
CXXFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))

NOVA_SRC := $(THIS_DIR)/kernel
ARCH_SRC := $(THIS_DIR)/$(ARCH_DIR)/kernel


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
	@echo "-> Linking $2"
	$(LD) $(LDFLAGS) -o $$@ $$^ libc/libc.a $3
	@echo "-> Executable $2 created"

#	@nm -n $$@
#	@readelf -S $$@
endef

define make-library
	libraries += $2
	sources += $1

$2: $(call source-to-object,$1)
	@echo "-> Linking $2"
	$(LD)  $(LDFLAGS) -o $$@ $$^ -r $3
	@echo "-> Library $2 created"
endef

include $(ARCH_DIR)/arch.mk
include libc/module.mk

objects = $(call source-to-object,$(sources))


#########################################
# Targets
#########################################
.PHONY: all
all: $(libraries) $(executables)

.PHONY: clean
clean:
	$(RM) $(objects) $(executables) $(libraries)


%.o: %.cpp
	@echo "-> Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	@echo "-> Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	@echo "-> Compiling $<"
	$(AS) $(ASFLAGS) $< -o $@