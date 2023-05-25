# Architectire, compiler and flags
TARGET := simux_kernel.elf

CC := g++
AS := nasm
ASM_FLAGS := -f elf32
CPP_FLAGS := -O2 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-rtti -Wall -Wextra -Werror
LD_FLAGS = -m elf_i386 -T linker.ld

# Define source and object directories
INC_DIR := -I include -I ../libc/include/
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*.asm)
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES += $(wildcard $(SRC_DIR)/*/*.cpp)

ASM_OBJS := $(patsubst ./%.asm, $(OBJ_DIR)/%.o, $(ASM_FILES))
CPP_OBJS := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))
LIBC_STATIC := ../libc/bin/libc.a

OUTPUT := $(BIN_DIR)/$(TARGET)


# Targets
.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(ASM_OBJS) $(CPP_OBJS)
	@echo "-> Linking object files"
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^ $(LIBC_STATIC)

	@echo "-> Binary information:"
	nm -n $(OUTPUT)

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
