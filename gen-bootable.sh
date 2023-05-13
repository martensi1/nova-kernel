# Create empty image
qemu-img create kernel.img 2M

# Partition the image with MBR partioning
# 1. Define disk geometry: 16 heads (max), 63 sectors (max) but only 4 cylinders to keep the image within 2MB (512 * 16 * 63 * 4 = 2 064 384 bytes)
# 2. Create a new primary partition with start sector 2048 (normal for GRUB) and end sector 4095
# 3. Set boot flag to the new partition to indicate that the device is bootable. Needed for BIOS to find the correct device to boot from
fdisk ./kernel.img << EOF
x
c
4
h
16
s
63
r
n
p
1
2048
4095
a
w
EOF

# Print image layout
fdisk -l -u ./kernel.img

sudo losetup -o 1048576 /dev/loop1 ./kernel.img
sudo mke2fs -t ext2 /dev/loop1
echo "Created loop device and formatted partition (EXT2)"

sudo mount /dev/loop1 /mnt/
echo "Mounted loop device"

cp -R bootdisk/* /mnt/
cp kernel/bin/simux_kernel.elf /mnt/boot/
echo "Copied GRUB configuration files and kernel to filesystem"

# sudo grub-install --root-directory=/mnt /dev/loop0
# sudo grub-mkconfig -o /mnt/boot/grub/grub.cfg

ls -lahR /mnt/

sudo umount /mnt/
echo "Unmounted loop device"

return

sudo grub --device-map=/dev/null << EOF
device (hd0) ./kernel.img
geometry (hd0) 4 16 63
root (hd0,0)
kernel /boot/simux_kernel.elf root=/dev/hda
setup (hd0)
quit
EOF
echo "GRUB installed!"

sudo losetup -d /dev/loop1
echo "Removed loop device"

echo "Done!"



