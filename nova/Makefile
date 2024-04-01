ROOTDIR := $(CURDIR)/../
include $(ROOTDIR)/make/functions.mk
include $(ROOTDIR)/make/colors.mk


#########################################
# Base variables
#########################################
CXX := g++
AS := nasm
MV := mv -f
RM := rm -f
SED := sed
TEST := test
MKDIR := mkdir -p


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
include $(ARCH_DIR)/flags.mk


#########################################
# Include directories
#########################################
THIS_DIR := .

CORE_INCLUDE := $(THIS_DIR)/include
LIBC_INCLUDE := $(THIS_DIR)/libc/include
ARCH_INCLUDE := $(THIS_DIR)/$(ARCH_DIR)/include

INCLUDE_DIRS := $(CORE_INCLUDE) $(LIBC_INCLUDE) $(ARCH_INCLUDE)
CXXFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))

CORE_SRC := $(THIS_DIR)/core
ARCH_SRC := $(THIS_DIR)/$(ARCH_DIR)/kernel


#########################################
# Modules
#########################################
executables :=
libraries :=
sources :=

define make-executable
	executables += $2
	sources += $1

$2: $(call source-to-object,$1)
	@echo "LD EXEC $$@"
	@$(LD) $(LDFLAGS) -o $$@ $$^ libc/libc.a $3
	@echo "$(GREEN)[EXEC]$(NC) -> $$@"

#	@nm -n $$@
#	@readelf -S $$@
endef

define make-library
	libraries += $2
	sources += $1

$2: $(call source-to-object,$1)
	@echo "LD LIB $$@"
	@$(LD) $(LDFLAGS) -o $$@ $$^ -r $3
	@echo "$(GREEN)[LIB]$(NC) -> $$@"
endef

define compile-rules
$(foreach f, $(local_src),$(call one-compile-rule,$(call source-to-object,$f),$f))
endef

define one-compile-rule
$(if $(filter %.c,$2),$(call compile-c,$1,$2),)
$(if $(filter %.cpp,$2),$(call compile-cpp,$1,$2),)
$(if $(filter %.asm,$2),$(call compile-asm,$1,$2),)
endef

define compile-c
$1: $2
	@mkdir -p $(dir $1)
	@echo "CXX $$<"
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef

define compile-cpp
$1: $2
	@mkdir -p $(dir $1)
	@echo "CXX $$<"
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef

define compile-asm
$1: $2
	@mkdir -p $(dir $1)
	@echo "AS $$<"
	@$(AS) $(ASFLAGS) $2 -o $1
endef

#modules = $(ARCH_DIR) libc

include $(ARCH_DIR)/modules.mk
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
