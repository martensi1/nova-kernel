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
# Base variables
#########################################
CXX := g++
AS := nasm
MV := mv -f
RM := rm -f
RMDIR := rm -rf
SED := sed
MKDIR := mkdir -p

ANSI_GREEN := \033[1;32m
ANSI_RED := \033[1;31m
ANSI_NC := \033[0m

DATETIME := $(shell date +%Y%m%d%H%M%S)

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
ifdef DEBUG
CXXFLAGS += -g
LDFLAGS += -g
else
CXXFLAGS += -O2
endif


#########################################
# Architecture selection
#########################################
ifeq ($(ARCH), i386)
ARCH_DIR := x86
else ifeq ($(ARCH), x86_64)
ARCH_DIR := x86
endif

ifdef ARCH_DIR
ARCH_DIR := archs/$(ARCH_DIR)
else
$(error Target architecture not defined or not supported!)
endif


#########################################
# Architecture flags
#########################################
include $(ARCH_DIR)/flags.mk


#########################################
# Include and source directories
#########################################
THIS_DIR := .

CORE_INCLUDE := $(THIS_DIR)/core/include
LIBC_INCLUDE := $(THIS_DIR)/libc/include
ARCH_INCLUDE := $(THIS_DIR)/$(ARCH_DIR)/kernel/include

INCLUDE_DIRS := $(CORE_INCLUDE) $(LIBC_INCLUDE) $(ARCH_INCLUDE)
CXXFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))

CORE_SRC := $(THIS_DIR)/core
ARCH_SRC := $(THIS_DIR)/$(ARCH_DIR)/kernel


#########################################
# Build path
#########################################
BUILD_DIR := ../bin/builds
OUT_DIR := $(BUILD_DIR)/$(ARCH)
#OUT_DIR := $(BUILD_DIR)/$(ARCH)-$(DATETIME)

BIN_DIR := $(OUT_DIR)/bin
OBJ_DIR := $(OUT_DIR)/obj

# $(call source-to-object, $(source_files))
source-to-object = $(addprefix $(OBJ_DIR)/, \
		$(subst .cpp,.o,$(filter %.cpp,$1)) \
		$(subst .asm,.o,$(filter %.asm,$1)) \
		$(subst .c,.o,$(filter %.c,$1)))

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
	@$(MKDIR) $(dir $1)
	@echo "CXX $$<"
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@

endef

# $(call compile-cpp, $(obj_path), $(src_path))
define compile-cpp
$1: $2
	@$(MKDIR) $(dir $1)
	@echo "CXX $$<"
	@$(CXX) $(CXXFLAGS) -c $$< -o $$@

endef

# $(call compile-asm, $(obj_path), $(src_path))
define compile-asm
$1: $2
	@$(MKDIR) $(dir $1)
	@echo "AS $$<"
	@$(AS) $(ASFLAGS) $2 -o $1

endef

# $(call link-executable, $(obj_files), $(exe_name), $(link_flags))
define link-executable
$2: $1
	@$(MKDIR) $(dir $2)
	@echo "LD EXEC $$@"
	@$(LD) $(LDFLAGS) -o $$@ $$^ $(BIN_DIR)/libc/libc.a $3
	@echo "$(ANSI_GREEN)[EXEC]$(ANSI_NC) -> $$@"

#	@nm -n $$@
#	@readelf -S $$@
endef

# $(call link-library, $(obj_files), $(lib_name), $(link_flags))
define link-library
$2: $1
	@$(MKDIR) $(dir $2)
	@echo "LD LIB $$@"
	@$(LD) $(LDFLAGS) -o $$@ $$^ -r $3
	@echo "$(ANSI_GREEN)[LIB]$(ANSI_NC) -> $$@"

endef


#########################################
# Module functions
#########################################
executables :=
libraries :=
sources :=

# $(call make-executable, $(source_files), $(out_path), $(link_flags))
define make-executable
executables += $(addprefix $(BIN_DIR)/,$2)
sources += $1
$(call link-executable,$(call source-to-object,$1),$(addprefix $(BIN_DIR)/,$2),$3)
endef

# $(call make-library, $(source_files), $(out_path), $(link_flags))
define make-library
libraries += $(addprefix $(BIN_DIR)/,$2)
sources += $1
$(call link-library,$(call source-to-object,$1),$(addprefix $(BIN_DIR)/,$2),$3)
endef

# Usage: $(subdirectory) Description: Returns the relative path to the current module
subdirectory = $(patsubst %/module.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))


#########################################
# Load modules
#########################################
include $(ARCH_DIR)/modules.mk
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
all: $(libraries) $(executables)

.PHONY: clean
clean:
	$(RMDIR) $(BUILD_DIR)
