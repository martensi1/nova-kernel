build-release:
	make -C ./libc all
	make -C ./kernel all

build-debug:
	make -C ./libc all DEBUG=1
	make -C ./kernel all DEBUG=1

package:
	@echo "Packaging kernel..."
	@sudo sh build-image.sh

emulate:
	@echo "Running QEMU..."
	@sudo sh start-qemu.sh

emulate-debug:
	@echo "Running QEMU in debug mode..."
	@sudo sh start-qemu.sh -debug-deamon

clean:
	@echo "Cleaning kernel..."
	make -C ./libc clean
	make -C ./kernel clean

test: clean build-release package emulate
debug: clean build-debug package emulate-debug

