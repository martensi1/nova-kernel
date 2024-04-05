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

source-to-object = $(addprefix $(OBJ_DIR)/, \
		$(subst .cpp,.o,$(filter %.cpp,$1)) \
		$(subst .asm,.o,$(filter %.asm,$1)) \
		$(subst .c,.o,$(filter %.c,$1)))

# $(call compile-src, $(src_path))
compile-src = $(call compile-src-to-obj,$(call source-to-object,$1),$1)

# $(call compile-src-to-obj, $(obj_path), $(src_path))
define compile-src-to-obj
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
GRUB_CFG_OUT := $(BOOT_DIR)/grub/grub.cfg
GRUB_CFG_BOOT_DATA :=

# $(call escape-sed-str, $(string))
escape-sed-str = $(subst /,\/,$1)

# $(call to-fs-boot-path, $(path))
to-fs-boot-path = /boot/$(notdir $1)

# $(call add-file-to-grub-cfg, $(file-type), $(file-fs-path)
define add-file-to-grub-cfg
	ifeq ($1, grub-start)
		GRUB_CFG_BOOT_DATA := $(GRUB_CFG_BOOT_DATA)multiboot $2\n\t
	else ifeq ($1, grub-module)
		GRUB_CFG_BOOT_DATA := $(GRUB_CFG_BOOT_DATA)module $2\n\t
	endif
endef

# $(call register-grub-file, $(file-type), $(path))
register-grub-file = $(call add-file-to-grub-cfg,$(strip $1),$(call to-fs-boot-path,$2))

$(GRUB_CFG_OUT): $(executables)
	@$(MKDIR) $(dir $@)
	@$(SED) -e 's/%MODULE-LIST%/$(call escape-sed-str,$(GRUB_CFG_BOOT_DATA))/g' grub-cfg.tmpl > $@
	@echo "$(ANSI_GREEN)[GRUB]$(ANSI_NC) -> $@"

package-boot: $(GRUB_CFG_OUT)
	@$(RM) $(BOOT_DIR)/*.elf
	@$(MKDIR) $(BOOT_DIR)
	@find $(BIN_DIR) -type f -name "*.elf" -exec cp {} $(BOOT_DIR)/ \;


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
$(call register-grub-file,$4,$(addprefix $(BIN_DIR)/,$2))
$(call link-executable,$(addprefix $(BIN_DIR)/,$2),$(call source-to-object,$1),$3)
endef

# $(call make-library, $(source_files), $(out_path), $(link_flags))
define make-library
	libraries += $(addprefix $(BIN_DIR)/,$2)
	sources += $1
$(call link-library,$(addprefix $(BIN_DIR)/,$2),$(call source-to-object,$1),$3)
endef


#########################################
# Load modules
#########################################

# Usage: $(subdirectory) Description: Returns the relative path to the current module
subdirectory = $(patsubst %/module.mk,%,                      \
               $(word                                         \
               $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

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
$(foreach f, $(strip $(call remove-duplicates,$(sources))),$(call compile-src,$f))
endef

$(eval $(generate-compile-rules))

#########################################
# General rules
#########################################
.PHONY: all
all: $(libraries) $(executables) package-boot