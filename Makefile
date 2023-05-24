all: 
	@echo "Compiling kernel..."
	make -C ./libc clean
	make -C ./libc all
	make -C ./kernel clean
	make -C ./kernel all

package:
	@echo "Packaging kernel..."
	@sudo sh build-image.sh

emulate:
	@echo "Running QEMU..."
	@sudo sh start-qemu.sh

emulate-debug:
	@echo "Running QEMU in debug mode..."
	@sudo sh start-qemu.sh -debug-deamon

test: all package emulate
debug: all package emulate-debug

