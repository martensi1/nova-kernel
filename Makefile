##############################################
# I386
##############################################
build-32:
	make -C ./libc all ARCH=i386
	make -C ./kernel all ARCH=i386

build-32-debug:
	make -C ./libc all ARCH=i386 DEBUG=1
	make -C ./kernel all ARCH=i386 DEBUG=1

emulate-32:
	make -C ./qemu run ARCH=i386

emulate-32-debug:
	make -C ./qemu debug ARCH=i386


##############################################
# x86_64
##############################################
build-64:
	make -C ./libc all ARCH=x86_64
	make -C ./kernel all ARCH=x86_64

build-64-debug:
	make -C ./libc all ARCH=x86_64 DEBUG=1
	make -C ./kernel all ARCH=x86_64 DEBUG=1

emulate-64:
	make -C ./qemu run ARCH=x86_64

emulate-debug-64:
	make -C ./qemu debug ARCH=x86_64

##############################################
# General
##############################################
package:
	@echo "Packaging kernel..."
	@sudo sh build-image.sh

clean:
	@echo "Cleaning kernel..."
	make -C ./libc clean
	make -C ./kernel clean


test-32: clean build-32 package emulate-32
test-64: clean build-64 package emulate-64

debug-32: clean build-32-debug package emulate-32-debug
debug-64: clean build-64-debug package emulate-64-debug

