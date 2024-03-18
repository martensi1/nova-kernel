######################################################
# QEMU architecture
######################################################
ifeq ($(ARCH), i386)
	QEMU_COMMAND := qemu-system-i386
else ifeq ($(ARCH), x86_64)
	QEMU_COMMAND := qemu-system-x86_64
else
$(error Simulation architecture not set! Please set ARCH variable to i386 or x86_64)
endif


######################################################
# QEMU parameters
######################################################
QEMU_MEMORY := 1024
QEMU_DRIVE := "format=raw,file=../bin/kernel.img"
QEMU_SERIAL := "file:serial.log"

QEMU_FLAGS := -m $(QEMU_MEMORY) -drive $(QEMU_DRIVE) -serial $(QEMU_SERIAL)

NON_DEBUG_FLAGS := -display curses
DEBUG_FLAGS := -s -S -vnc :5 -daemonize


######################################################
# GDB parameters
######################################################
GDB := gdb
GDB_FLAGS := -ex "target remote localhost:1234"
GDB_FLAGS += -ex "symbol-file ../bin/nova_kernel.elf"
GDB_FLAGS += -ex "break kmain" -ex "continue"


######################################################
# Targets
######################################################
.PHONY: run debug

run:
	@echo "Starting QEMU (architecture: $(ARCH), debug: true)..."
	@$(QEMU_COMMAND) $(QEMU_FLAGS) $(NON_DEBUG_FLAGS)

debug:
	@echo "Starting QEMU (architecture: $(ARCH), debug: false)..."
	@$(QEMU_COMMAND) $(QEMU_FLAGS) $(DEBUG_FLAGS)
	@$(GDB) $(GDB_FLAGS)
