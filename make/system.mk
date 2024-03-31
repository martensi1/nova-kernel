nova_define = $(eval $1 := $2)$(eval export $1)

nova_define(CXX, g++)
nova_define(AS, nasm)
nova_define(MV, mv -f)
nova_define(RM, rm -f)
nova_define(SED, sed)