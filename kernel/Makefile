# Architectire and target
ARCH := x86
TARGET := kernel.elf

# Compiler and flags
CC := g++
AS := nasm
ASM_FLAGS := -f elf32
CPP_FLAGS := -O2 -ffreestanding -nostdlib -Wall -Wextra -Werror
LD_FLAGS = -m elf_i386 -T linker.ld

# Directories
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

# Source files
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
ASM_OBJS := $(patsubst ./%.asm, $(OBJ_DIR)/%.o, $(ASM_FILES))
CPP_OBJS := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

# Build rule for assembly files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(OBJ_DIR)
	$(AS) $(ASM_FLAGS) $< -o $@

# Build rule for C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CPP_FLAGS) -c $< -o $@

# Default target
all: $(TARGET)

$(TARGET): $(ASM_OBJS) $(CPP_OBJS)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
