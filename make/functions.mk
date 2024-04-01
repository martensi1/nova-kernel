##################################################
# source-to-object
# - $1: list of source files
# - returns: list of object files 
#            (same path but with .o extension)
##################################################
BINARY_DIR = ../bin/$(ARCH)/build/
source-dir-to-binary-dir = $(addprefix $(BINARY_DIR)/, $1)

source-to-object = $(call source-dir-to-binary-dir, \
				   $(subst .cpp,.o,$(filter %.cpp,$1)) \
                   $(subst .asm,.o,$(filter %.asm,$1)) \
				   $(subst .c,.o,$(filter %.c,$1)))


##################################################
# subdirectory
# - returns: the name of the current subdirectory
##################################################
subdirectory = $(patsubst %/module.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
