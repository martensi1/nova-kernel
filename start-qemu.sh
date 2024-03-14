QEMU_MEMORY=1024
QEMU_DRIVE="format=raw,file=./bin/kernel.img"
ELF_FILE="./bin/simux_kernel.elf"

sh ./kill-qemu.sh

if [ $1 = "-debug-deamon" ]; then
    qemu-system-i386 \
    -m $QEMU_MEMORY \
    -drive "$QEMU_DRIVE" \
    -s \
    -S \
    -vnc :5 \
    -daemonize

    sleep 1

    gdb \
    -ex "target remote localhost:1234" \
    -ex "symbol-file ./bin/simux_kernel.elf" \
    -ex "break kmain" \
    -ex "continue"
else
    qemu-system-i386 \
    -m $QEMU_MEMORY \
    -drive "$QEMU_DRIVE" \
    -display curses \
    -serial /dev/tty
fi

sh ./kill-qemu.sh

#reset
exit 0