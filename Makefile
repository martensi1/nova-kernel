all: 
	@echo "Compiling kernel..."
	make -C ./kernel clean
	make -C ./kernel
