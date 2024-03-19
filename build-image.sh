#!/bin/sh

# Makes script fail if any command fails
set -e

# Change to script directory
cd "$(dirname "$0")"

# Logging
GREEN='\033[1;32m'
RED='\033[1;31m'
NC='\033[0m'

checkpoint () {
    echo "${GREEN}[OK]${NC}: $1"
}

error () {
    echo "${RED}[ERROR]${NC}: $1"
    exit 1
}

############################
# Print welcome message
############################
VERSION='0.1.0'

echo "############################################"
echo "#    Nova OS - Image Builder (v${VERSION})  "
echo "############################################"


############################
# Check if root
############################
if [ "$(id -u)" -ne 0 ]; then
    echo "Please run as root"
    exit 1
fi


############################
# Check dependencies
############################
DEPENDENCIES="qemu-utils grub2 grub2-common fdisk e2fsprogs"

for DEPENDENCY in $DEPENDENCIES; do
    if ! dpkg -l | grep -q $DEPENDENCY; then
        echo "Dependency not found: $DEPENDENCY"
        error "Please install the required dependencies"
    fi
done

checkpoint "Build tools found"


############################
# Create empty image
############################
IMAGE_PATH="$OUTPUT_DIR/kernel.img"
IMAGE_SIZE="30M"

if [ -f "$IMAGE_PATH" ]; then
    rm -f "$IMAGE_PATH"

    if [ -f "$IMAGE_PATH" ]; then
        echo "Failed to delete old image file: $IMAGE_PATH"
        exit 1
    fi
fi

mkdir -p "$(dirname "$IMAGE_PATH")"
qemu-img create "$IMAGE_PATH" "$IMAGE_SIZE"

checkpoint "Empty image created"


############################
# Partition the image
############################
DISK_GEOMETRY_HEADS="16"
DISK_GEOMETRY_SECTORS="63"
DISK_GEOMETRY_CYLINDERS="12"

PARTITION_FILE_SYSTEM="ext4"
PARITION_SECTOR_OFFSET="4096"
PARTITION_BYTES_OFFSET=$(( $PARITION_SECTOR_OFFSET * 512 ))

(
  echo x # Expert mode
  echo c # Set cylinder count
  echo $DISK_GEOMETRY_CYLINDERS
  echo h # Set head count
  echo $DISK_GEOMETRY_HEADS
  echo s # Set sector count
  echo $DISK_GEOMETRY_SECTORS
  echo r # Return to main menu
  echo n # Add a new partition
  echo p # Primary partition
  echo 1 # Partition number
  echo $PARITION_SECTOR_OFFSET
  echo # Last sector (Accept default: last)
  echo a # Add boot flag to partition
  echo w # Write changes
) | fdisk "$IMAGE_PATH" > /dev/null

checkpoint "Image partitioned"


############################
# Setup loop devices, format
# and mount the partitions
############################
LOOP_DEVICE="/dev/loop0"
LOOP_DEVICE_PARTITION="/dev/loop1"
MOUNT_DIR="/mnt"

# Cleanup from previous runs
umount "$MOUNT_DIR" > /dev/null 2>&1 || true
sleep 0.1
losetup -d "$LOOP_DEVICE_PARTITION" > /dev/null 2>&1 || true
sleep 0.1
losetup -d "$LOOP_DEVICE" > /dev/null 2>&1 || true
sleep 0.1

# Setup loop devices
losetup "$LOOP_DEVICE" "$IMAGE_PATH"
losetup -o $PARTITION_BYTES_OFFSET "$LOOP_DEVICE_PARTITION" "$IMAGE_PATH"
sleep 0.1

# Format partition with a filesystem and mount it
mke2fs -t "$PARTITION_FILE_SYSTEM" "$LOOP_DEVICE_PARTITION" > /dev/null
mount "$LOOP_DEVICE_PARTITION" "$MOUNT_DIR"
rm -rf "$MOUNT_DIR"/*

checkpoint "Filesystem formatted"


############################
# Install GRUB
############################
GRUB_TARGET="i386-pc"
GRUB_MODULES="normal part_msdos ext2 multiboot"
GRUB_VERSION=$(grub-install --version | grep -oP "grub-install \(GRUB\) \K[0-9]+\.[0-9]+")

grub-install --target "$GRUB_TARGET" --no-floppy --root-directory="$MOUNT_DIR" "$LOOP_DEVICE" --modules "$GRUB_MODULES"
checkpoint "GRUB v${GRUB_VERSION} installed"


############################
# Kernel multiboot test
############################
ELF_FILE="./kernel/bin/nova_kernel.elf"
grub-file --is-x86-multiboot "$ELF_FILE"

checkpoint "Kernel multiboot check"


############################
# Install kernel
############################
cp "$ELF_FILE" "$MOUNT_DIR/boot/"
checkpoint "Kernel copied"


############################
# Add system files
############################
cp -R sysroot/* /mnt/
checkpoint "System files copied"


############################
# Unmount and cleanup
############################
umount "$MOUNT_DIR"
sleep 0.1
losetup -d "$LOOP_DEVICE_PARTITION"
sleep 0.1
losetup -d "$LOOP_DEVICE"
sleep 0.1


############################
# Finalize
############################
echo "Image produced (size: $IMAGE_SIZE, MD5: $(md5sum "$IMAGE_PATH" | cut -d ' ' -f 1))"

# Copy ELF to output directory for debugging
cp "$ELF_FILE" "$OUTPUT_DIR/"