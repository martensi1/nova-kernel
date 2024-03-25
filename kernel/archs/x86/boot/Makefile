TARGET := loader.bin



ASM_FLAGS += -f elf32
CPP_FLAGS += -m32 -DNOVA_ARCH_I386=1
LD_FLAGS += -m elf_i386 -T linker.ld

INC_DIR := -I ../ -I ../../../../ -I ../../../../../libc/include/
OBJ_DIR := obj
BIN_DIR := bin
LIBC_STATIC := ../../../../../libc/bin/libc.a

ASM_FILES := $(wildcard ./*.asm)
CPP_FILES := $(wildcard ./*.cpp)

CPP_FILES += ../../../tty.cpp
CPP_FILES += ../../../console/vgacon.cpp
CPP_FILES += ../../../kernel.cpp



ASM_OBJS := $(patsubst ./%.asm, $(OBJ_DIR)/%.o, $(ASM_FILES))
CPP_OBJS := $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

OUTPUT := $(BIN_DIR)/$(TARGET)


# Targets
.PHONY: all clean

test:
	@echo $(CPP_FILES)
	@echo $(CPP_OBJS)

all: $(OUTPUT)

$(OUTPUT): $(ASM_OBJS) $(CPP_OBJS)
	@echo "-> Linking object files"
	@mkdir -p $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^ $(LIBC_STATIC)

	@echo "-> Done!"

$(OBJ_DIR)/%.o: %.asm
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	$(AS) $(ASM_FLAGS) $< -o $(OBJ_DIR)/$*.o

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)/$(dir $*)

	@echo "-> Compiling $<"
	$(CPP) $(CPP_FLAGS) $(INC_DIR) -c $< -o $(OBJ_DIR)/$*.o


clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
