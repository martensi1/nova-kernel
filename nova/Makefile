# Nova OS
# Copyright (C) 2024 Simon Alm Mårtensson
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#

#########################################
# Programs, commands and constants
#########################################
CXX := g++
AS := nasm
MV := mv -f
CP := cp
RM := rm -f
RMDIR := rm -rf
MKDIR := mkdir -p
SED := sed

ANSI_GREEN := \033[1;32m
ANSI_RED := \033[1;31m
ANSI_NC := \033[0m

DATETIME := $(shell date +%Y%m%d%H%M%S)

# Output directories
BIN_DIR := ../bin
BUILD_DIR := $(BIN_DIR)/build
OUT_DIR := $(BUILD_DIR)
#OUT_DIR := $(BUILD_DIR)/$(ARCH)-$(DATETIME)

BIN_DIR := $(OUT_DIR)/bin
OBJ_DIR := $(OUT_DIR)/obj
BOOT_DIR := $(OUT_DIR)/boot


#########################################
# Architecture selection
#########################################
ifndef ARCH
$(error Target architecture not defined!)
endif

include arch-map.mk
archdir := $(ARCH_DIR_$(ARCH))

ifeq ($(strip $(archdir)),)
$(error Target architecture '$(ARCH)' not supported!)
endif

ARCH_PATH := archs/$(archdir)


#########################################
# Compiler flags
#########################################
CXXFLAGS := -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs \
-fno-use-cxa-atexit -fno-rtti -Wall -Wextra -Werror -std=c++11
LDFLAGS := -z noexecstack
ASFLAGS :=

ifdef DEBUG
CXXFLAGS += -g
LDFLAGS += -g
else
CXXFLAGS += -O2
endif

include $(ARCH_PATH)/flags.mk


#########################################
# Include and source directories
#########################################
core_include := core/include
libc_include := libc/include
arch_include := $(ARCH_PATH)/kernel/include

include_dirs := $(core_include) $(libc_include) $(arch_include)
CXXFLAGS += $(addprefix -I ,$(include_dirs))

CORE_SRC := core
ARCH_SRC := $(ARCH_PATH)/kernel


#########################################
# Build functions
#########################################

# $(call compile-src, $(obj_path), $(src_path))
define compile-src
$(if $(filter %.c,$2),$(call compile-c,$1,$2),)
$(if $(filter %.cpp,$2),$(call compile-cpp,$1,$2),)
$(if $(filter %.asm,$2),$(call compile-asm,$1,$2),)
endef

# $(call compile-c, $(obj_path), $(src_path))
define compile-c
$1: $2
	@echo "CXX $$<"
	@$(MKDIR) $(dir $1)
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@

endef

# $(call compile-cpp, $(obj_path), $(src_path))
define compile-cpp
$1: $2
	@echo "CXX $$<"
	@$(MKDIR) $(dir $1)
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@

endef

# $(call compile-asm, $(obj_path), $(src_path))
define compile-asm
$1: $2
	@echo "AS $$<"
	@$(MKDIR) $(dir $1)
	@$(AS) $(ASFLAGS) $2 -o $1

endef

# $(call link-executable, $(obj_files), $(out_path), $(link_flags))
define link-executable
$1: $2
	@echo "LD EXEC $$@"
	@$(MKDIR) $(dir $1)
	@$(LD) $(LDFLAGS) -o $$@ $$^ $(BIN_DIR)/libc/libc.a $3
	@echo "$(ANSI_GREEN)[EXEC]$(ANSI_NC) -> $$@"

endef

# $(call link-library, $(obj_files), $(out_path), $(link_flags))
define link-library
$1: $2
	@echo "LD LIB $$@"
	@$(MKDIR) $(dir $1)
	@$(LD) $(LDFLAGS) -o $$@ $$^ -r $3
	@echo "$(ANSI_GREEN)[LIB]$(ANSI_NC) -> $$@"

endef


#########################################
# GRUB functions
#########################################
grubdata :=
grubcopy :=

# $(call boot-path, $(path))
fs-boot-path = /boot/$(notdir $1)

# $(call escape-sed-str, $(string))
escape-sed-str = $(subst /,\/,$1)

define copy-to-boot
$(BOOT_DIR)/$(notdir $1): $1
	@$(MKDIR) $(dir $$@)
	@$(CP) $$^ $(BOOT_DIR)/
endef

# $(call add-grub-file, $(type), $(path))
define add-grub-file
	type = $(strip $1)
	fspath = $(call fs-boot-path,$2)

	ifeq ($(type), grub-start)
		grubdata := $(grubdata)multiboot $(fspath)\n\t
	else ifeq ($(type), grub-module)
		grubdata := $(grubdata)module $(fspath)\n\t
	endif


	grubcopy += $(BOOT_DIR)/$(notdir $2)

$(path): $2
	@$(MKDIR) $(dir $$@)
	@$(CP) $$^ $(BOOT_DIR)/

endef







#########################################
# GRUB config functions
#########################################
grubdata :=
grubcopy :=

define register-grub-file
ifeq ($(strip $1), grub-start)
grubdata := $(grubdata)multiboot $(call boot-path,$2)\n\t
else ifeq ($(strip $1), grub-module)
grubdata := $(grubdata)module $(call boot-path,$2)\n\t
endif

grubcopy += $(BOOT_DIR)/$(notdir $2)

$(BOOT_DIR)/$(notdir $2): $2
	@$(MKDIR) $(dir $$@)
	@$(CP) $$^ $(BOOT_DIR)/

endef


$(BOOT_DIR)/grub/grub.cfg: $(executables)
	@$(MKDIR) $(dir $@)
	@$(SED) -e 's/%MODULE-LIST%/$(call escape-sed-str,$(grubdata))/g' grub-cfg.tmpl > $@



#########################################
# Module functions
#########################################
executables :=
libraries :=
sources :=

# $(call source-to-object, $(source_files))
source-to-object = $(addprefix $(OBJ_DIR)/, \
		$(subst .cpp,.o,$(filter %.cpp,$1)) \
		$(subst .asm,.o,$(filter %.asm,$1)) \
		$(subst .c,.o,$(filter %.c,$1)))

# $(call make-executable, $(source_files), $(out_path), $(link_flags), $(grub-type))
define make-executable
executables += $(addprefix $(BIN_DIR)/,$2)
sources += $1
$(call add-grub-file,$4,$(addprefix $(BIN_DIR)/,$2))
$(call link-executable,$(addprefix $(BIN_DIR)/,$2),$(call source-to-object,$1),$3)
endef

# $(call make-library, $(source_files), $(out_path), $(link_flags))
define make-library
libraries += $(addprefix $(BIN_DIR)/,$2)
sources += $1
$(call link-library,$(addprefix $(BIN_DIR)/,$2),$(call source-to-object,$1),$3)
endef

# Usage: $(subdirectory) Description: Returns the relative path to the current module
subdirectory = $(patsubst %/module.mk,%,                      \
               $(word                                         \
               $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))


#########################################
# Load modules
#########################################
include $(ARCH_PATH)/modules.mk
include libc/module.mk


#########################################
# Generate compile rules
#########################################

# $(call uniq, $(list))
define remove-duplicates =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

# $(call generate-compile-rules)
define generate-compile-rules
$(foreach f, $(strip $(call remove-duplicates,$(sources))),$(call compile-src,$(call source-to-object,$f),$f))
endef

$(eval $(generate-compile-rules))

#########################################
# General rules
#########################################
.PHONY: all
all: $(libraries) $(executables) $(BOOT_DIR)/grub/grub.cfg $(grubcopy)

.PHONY: clean
clean:
	$(RMDIR) $(BUILD_DIR)
