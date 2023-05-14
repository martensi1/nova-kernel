all: 
	@echo "Compiling kernel..."
	make -C ./kernel clean
	make -C ./kernel all

package:
	@echo "Packaging kernel..."
	@sudo sh gen-bootable.sh

emulate:
	@echo "Running QEMU..."
	@sudo sh start-qemu.sh

test: all package emulate

