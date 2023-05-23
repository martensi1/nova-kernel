QEMU_MEMORY=1024
QEMU_DRIVE="format=raw,file=./bin/kernel.img"

sh ./kill-qemu.sh

if [ $1 = "-debug-deamon" ]; then
    qemu-system-i386 -m $QEMU_MEMORY -drive "$QEMU_DRIVE" -s -S -vnc :5 -daemonize
    sleep 1
    gdb -ex "add-symbol-file ./bin/simux_kernel.elf 0x100000" -ex "target remote localhost:1234" -ex "continue" -ex "break gdt_initialize"
else
    qemu-system-i386 -m $QEMU_MEMORY -drive "$QEMU_DRIVE" -curses -serial /dev/tty
fi

sh ./kill-qemu.sh

reset
exit 0