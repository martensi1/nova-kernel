nova_define = $(eval $1 := $2)$(eval export $1)

source-to-object = $(subst .cpp,.o,$(filter %.cpp,$1)) \
                   $(subst .asm,.o,$(filter %.asm,$1)) \
				   $(subst .c,.o,$(filter %.c,$1))

subdirectory = $(patsubst %/module.mk,%,                        \
                 $(word                                         \
                   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))