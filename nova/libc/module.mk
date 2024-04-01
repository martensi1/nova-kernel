local_src := 
local_src += $(wildcard $(subdirectory)/*.c)
local_src += $(wildcard $(subdirectory)/**/*.c)

$(eval $(call make-library, $(local_src), $(subdirectory)/libc.a))
$(eval $(compile-rules))