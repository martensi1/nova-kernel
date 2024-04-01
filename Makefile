##############################################
# I386
##############################################
build-32:
	$(MAKE) -C ./nova all ARCH=i386

build-32-debug:
	$(MAKE) -C ./nova all ARCH=i386 DEBUG=1

emulate-32:
	$(MAKE) -C ./qemu run ARCH=i386

emulate-32-debug:
	$(MAKE) -C ./qemu debug ARCH=i386


##############################################
# x86_64
##############################################
build-64:
	$(MAKE) -C ./nova all ARCH=x86_64

build-64-debug:
	$(MAKE) -C ./nova all ARCH=x86_64 DEBUG=1

emulate-64:
	$(MAKE) -C ./qemu run ARCH=x86_64

emulate-debug-64:
	$(MAKE) -C ./qemu debug ARCH=x86_64

##############################################
# General
##############################################
package:
	@sudo ./build-image.sh

clean:
	$(MAKE) -C ./nova clean ARCH=i386


test-32: clean build-32 package emulate-32
test-64: clean build-64 package emulate-64

debug-32: build-32-debug package emulate-32-debug
debug-64: build-64-debug package emulate-64-debug

