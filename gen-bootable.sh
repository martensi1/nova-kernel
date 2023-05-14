#!/bin/sh


# Define variables
IMAGE_PATH="./kernel.img"
IMAGE_SIZE="8M"
MOUNT_DIR="/mnt"

DISK_GEOMETRY_HEADS="16"
DISK_GEOMETRY_SECTORS="63"
DISK_GEOMETRY_CYLINDERS="4"

PARTITION_FILE_SYSTEM="ext2"
PARITION_SECTOR_OFFSET="2048"
PARTITION_BYTES_OFFSET=$(( $PARITION_SECTOR_OFFSET * 512 ))

LOOP_DEV="/dev/loop0"
LOOP_DEV_PART="/dev/loop1"


# Create empty image
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
losetup "$LOOP_DEV" "$IMAGE_PATH"
losetup -o $PARTITION_BYTES_OFFSET "$LOOP_DEV_PART" "$IMAGE_PATH"
sleep 0.1


# Format the partition
echo "-> Formatting partition"
mke2fs -t "$PARTITION_FILE_SYSTEM" "$LOOP_DEV_PART"


# Mount the partition
echo "-> Mounting partition"
mount "$LOOP_DEV_PART" "$MOUNT_DIR"
rm -rf "$MOUNT_DIR"/*


echo "-> Install GRUB"
mkdir -p /mnt/boot/grub

grub-install --target i386-pc --no-floppy --root-directory=/mnt /dev/loop0
#cp -R bootdisk/* /mnt/
cat > /mnt/boot/grub/device.map <<EOF
(hd0)   /dev/loop0
(hd0,1) /dev/loop1
EOF


echo "-> Copying kernel"
cp kernel/bin/simux_kernel.elf /mnt/boot/
cat /mnt/boot/grub/device.map
rm -rf /mnt/boot/grub/device.map

ls -lah /mnt/boot/grub


echo "-> Detaching loop devices..."
umount /mnt/
sleep 0.1
losetup -d /dev/loop1
sleep 0.1
kpartx -v -d /dev/loop0
sleep 0.1
losetup -d /dev/loop0
sleep 0.1

echo "-> MD5 checksum:"
md5sum kernel.img

echo "-> Done!"



