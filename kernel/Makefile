# Architectire, compiler and flags
ARCH := x86
TARGET := simux_kernel.elf

CC := g++
AS := nasm
ASM_FLAGS := -f elf32
CPP_FLAGS := -O2 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-rtti -Wall -Wextra -Werror
LD_FLAGS = -m elf_i386 -T linker.ld

# Define source and object directories
INC_DIR := ../include
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

ASM_OBJS := $(patsubst ./%.asm, $(OBJ_DIR)/%.o, $(ASM_FILES))
CPP_OBJS := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

OUTPUT := $(BIN_DIR)/$(TARGET)


# Build rule for assembly files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(OBJ_DIR)
	$(AS) $(ASM_FLAGS) $< -o $@

# Build rule for C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CPP_FLAGS) -c $< -o $@ -I $(INC_DIR)

# Default target
all: $(OUTPUT)

$(OUTPUT): $(ASM_OBJS) $(CPP_OBJS)
	@echo "-> Linking object files"
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^

	@echo "-> Printing binary information:"
	nm -n $(OUTPUT)

	@echo "-> Done!"


# Clean rule
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
