qemu-system-i386 -m 1024 -s -drive format=raw,file=./kernel.img -curses -serial /dev/tty  -redir tcp:2323::23
#qemu-system-i386 -m 1024 -s -hda ./kernel.img  -curses -serial /dev/tty  -redir tcp:2323::23


# KNOWN TO WORK
# qemu-system-i386 -m 1024 -s -hda ./linux-0.2.img  -curses -serial /dev/tty  -redir tcp:2323::23
# qemu-system-i386 -m 1024 -s -drive format=raw,file=./kernel.img -curses -serial /dev/tty  -redir tcp:2323::23