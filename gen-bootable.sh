#!/bin/sh


# Define variables
IMAGE_PATH="./kernel.img"
IMAGE_SIZE="8M"

DISK_GEOMETRY_HEADS="16"
DISK_GEOMETRY_SECTORS="63"
DISK_GEOMETRY_CYLINDERS="12"

PARTITION_FILE_SYSTEM="ext4"
PARITION_SECTOR_OFFSET="4096"
PARTITION_BYTES_OFFSET=$(( $PARITION_SECTOR_OFFSET * 512 ))

LOOP_DEVICE="/dev/loop0"
LOOP_DEVICE_PARTITION="/dev/loop1"
MOUNT_DIR="/mnt"

GRUB_TARGET="i386-pc"


# Create empty image
sudo rm -f "$IMAGE_PATH"
qemu-img create "$IMAGE_PATH" "$IMAGE_SIZE"


# Partition the image with fdisk with MBR (Master Boot Record) partition table
# 1. Define disk geometry: 16 heads (max), 63 sectors (max) but only 4 cylinders to keep the image within 2MB (512 * 16 * 63 * 4 = 2 064 384 bytes)
# 2. Create a new primary partition with start sector 2048 (normal for GRUB) and end sector 4095
# 3. Set boot flag to the new partition to indicate that the device is bootable. Needed for BIOS to find the correct device to boot from
fdisk "$IMAGE_PATH" << EOF
x
c
$DISK_GEOMETRY_CYLINDERS
h
$DISK_GEOMETRY_HEADS
s
$DISK_GEOMETRY_SECTORS
r
n
p
1
$PARITION_SECTOR_OFFSET

a
w
EOF

fdisk -l -u "$IMAGE_PATH"


# Associate the image with a loop device and create a partition device
echo "-> Attaching loop devices"
losetup "$LOOP_DEVICE" "$IMAGE_PATH"
losetup -o $PARTITION_BYTES_OFFSET "$LOOP_DEVICE_PARTITION" "$IMAGE_PATH"
sleep 0.1

# Format the partition
echo "-> Formatting partition"
mke2fs -t "$PARTITION_FILE_SYSTEM" "$LOOP_DEVICE_PARTITION"


# Mount the partition
echo "-> Mounting partition"
mount "$LOOP_DEVICE_PARTITION" "$MOUNT_DIR"
rm -rf "$MOUNT_DIR"/*


echo "-> Install GRUB"
mkdir -p /mnt/boot/grub

grub-install --version
grub-install --target "$GRUB_TARGET" --no-floppy --root-directory="$MOUNT_DIR" "$LOOP_DEVICE" --modules "normal part_msdos ext2 multiboot"
cp -R bootdisk/* /mnt/


echo "-> Copying kernel"
cp kernel/bin/simux_kernel.elf /mnt/boot/
#ls -lahR /mnt/
cat /mnt/boot/grub/grub.cfg


echo "-> Detaching loop devices..."
umount "$MOUNT_DIR"
sleep 0.1
losetup -d "$LOOP_DEVICE_PARTITION"
sleep 0.1
losetup -d "$LOOP_DEVICE"
sleep 0.1

fdisk -l -u "$IMAGE_PATH"

echo "-> MD5 checksum:"
md5sum "$IMAGE_PATH"

echo "-> Done!"



