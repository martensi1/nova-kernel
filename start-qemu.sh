qemu-system-i386 -m 1024 -s -drive format=raw,file=./bin/kernel.img -curses -serial /dev/tty  -redir tcp:2323::23
reset