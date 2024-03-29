export CPP := g++
export AS := nasm

export NOVA_INCLUDE := $(shell pwd)


# Architectire, compiler and flags
TARGET := nova_kernel.elf

ASM_FLAGS := 
CPP_FLAGS := -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-use-cxa-atexit -fno-rtti -Wall -Wextra -Werror -std=c++11
LD_FLAGS = -T linker.ld -z noexecstack

ifeq ($(ARCH), i386)
	ASM_FLAGS += -f elf32
	CPP_FLAGS += -m32 -DNOVA_ARCH_I386=1
	LD_FLAGS += -m elf_i386
else ifeq ($(ARCH), x86_64)
	ASM_FLAGS += -f elf64
	CPP_FLAGS += -m64 -DNOVA_ARCH_X86_64=1
	LD_FLAGS += -m elf_x86_64
else
#$(error Target architecture not defined! Please set ARCH to i386 or x86_64)
endif

ifeq ($(DEBUG), 1)
	CPP_FLAGS += -g
	LD_FLAGS += -g
else
	CPP_FLAGS += -O2
endif

# Define source and object directories
INC_DIR := -I ./include/ -I ../libc/include/ -I ./archs/x86/include/ -I ./archs/x86/
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

#rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*.asm)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*/*.asm)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*/*/*.asm)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*/*/*/*.asm)
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES += $(wildcard $(SRC_DIR)/*/*.cpp)
CPP_FILES += $(wildcard $(SRC_DIR)/*/*/*.cpp)
CPP_FILES += $(wildcard $(SRC_DIR)/*/*/*/*.cpp)
CPP_FILES += $(wildcard $(SRC_DIR)/*/*/*/*/*.cpp)

ASM_OBJS := $(patsubst ./%.asm, $(OBJ_DIR)/%.o, $(ASM_FILES))
CPP_OBJS := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))
#LIBC_STATIC := ../libc/bin/libc.a

OUTPUT := $(BIN_DIR)/$(TARGET)


# Targets
.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(ASM_OBJS) $(CPP_OBJS)
	@echo "-> Linking object files"
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^ $(LIBC_STATIC)

#	@nm -n $(OUTPUT) >> $(BIN_DIR)/symbols.txt
#	@readelf -S $(OUTPUT) >> $(BIN_DIR)/sections.txt
	@echo "-> Done!"

$(OBJ_DIR)/%.o: %.asm
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	$(AS) $(ASM_FLAGS) $< -o $(OBJ_DIR)/$*.o

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)/$(dir $*)

	@echo "-> Compiling $<"
	$(CC) $(CPP_FLAGS) $(INC_DIR) -c $< -o $(OBJ_DIR)/$*.o


clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
