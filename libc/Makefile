# Compiler and flags
TARGET := libc.a

CC := g++
ASM_FLAGS := -f elf32
CPP_FLAGS := -O2 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -nodefaultlibs -fno-rtti -Wall -Wextra -Werror
LD_FLAGS = -m elf_i386

# Define source and object directories
INC_DIR := -I include
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

C_FILES := $(wildcard $(SRC_DIR)/*/*.c)
C_OBJS := $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(C_FILES))

OUTPUT := $(BIN_DIR)/$(TARGET)


# Targets
.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(C_OBJS)
	@echo "-> Linking object files"
	$(LD) $(LD_FLAGS) -o $@ $^

	@echo "-> Binary information:"
	nm -n $(OUTPUT)

	@echo "-> Done!"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)/$(dir $*)

	@echo "-> Compiling $<"
	$(CC) $(CPP_FLAGS) $(INC_DIR) -c $< -o $(OBJ_DIR)/$*.o


clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
