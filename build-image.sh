# Setup
cd "$(dirname "$0")"

ELF_FILE="./kernel/bin/nova_kernel.elf"
OUTPUT_DIR="./bin"

IMAGE_PATH="$OUTPUT_DIR/kernel.img"
IMAGE_SIZE="30M"

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
GRUB_MODULES="normal part_msdos ext2 multiboot"

# Create empty image
if [ -f "$IMAGE_PATH" ]; then
    rm -f "$IMAGE_PATH"

    if [ -f "$IMAGE_PATH" ]; then
        echo "Failed to delete old image file: $IMAGE_PATH"
        exit 1
    fi
fi

mkdir -p "$(dirname "$IMAGE_PATH")"
qemu-img create "$IMAGE_PATH" "$IMAGE_SIZE"

# Partition the image
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


# Associate image and its partition with loop devices
losetup "$LOOP_DEVICE" "$IMAGE_PATH"
losetup -o $PARTITION_BYTES_OFFSET "$LOOP_DEVICE_PARTITION" "$IMAGE_PATH"
sleep 0.1

# Format the partition and mount it
mke2fs -t "$PARTITION_FILE_SYSTEM" "$LOOP_DEVICE_PARTITION"
mount "$LOOP_DEVICE_PARTITION" "$MOUNT_DIR"
rm -rf "$MOUNT_DIR"/*

# Install GRUB
grub-install --version
grub-install --target "$GRUB_TARGET" --no-floppy --root-directory="$MOUNT_DIR" "$LOOP_DEVICE" --modules "$GRUB_MODULES"

# Install kernel
cp "$ELF_FILE" "$MOUNT_DIR/boot/"
grub-file --is-x86-multiboot "$MOUNT_DIR/boot/nova_kernel.elf"

if [ $? -ne 0 ]; then
    echo "Kernel is not multiboot compatible!"
    exit 1
else
    echo "Kernel is multiboot compatible!"
fi

# Copy OS files
cp -R sysroot/* /mnt/

# Unmount and cleanup
umount "$MOUNT_DIR"
sleep 0.1
losetup -d "$LOOP_DEVICE_PARTITION"
sleep 0.1
losetup -d "$LOOP_DEVICE"
sleep 0.1

# Show image info
fdisk -l -u "$IMAGE_PATH"
md5sum "$IMAGE_PATH"

# Copy ELF to output directory for debugging
cp "$ELF_FILE" "$OUTPUT_DIR/"

echo "Done!"



