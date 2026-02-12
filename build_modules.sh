#!/bin/bash

echo "=== Building SvetOS Modules ==="

MODULES_DIR="modules"
BUILD_DIR="build/modules"
CC="gcc"
CFLAGS="-ffreestanding -nostdlib -Iinclude -O2 -Wall -Wextra"
LD="ld"
LDFLAGS="-nostdlib"

# Создаем директории
mkdir -p $BUILD_DIR/drivers/storage/ata
mkdir -p $BUILD_DIR/sys/fs/initrd_fs

echo "1. Building ATA disk driver..."
$CC $CFLAGS -c $MODULES_DIR/drivers/storage/ata/ata_module.c -o $BUILD_DIR/drivers/storage/ata/ata_module.o
$LD $LDFLAGS -o $MODULES_DIR/ata.kmod $BUILD_DIR/drivers/storage/ata/ata_module.o

echo "2. Building InitRD filesystem..."
$CC $CFLAGS -c $MODULES_DIR/sys/fs/initrd_fs/initrd_fs_module.c -o $BUILD_DIR/sys/fs/initrd_fs/initrd_fs.o  
$LD $LDFLAGS -o $MODULES_DIR/initrd_fs.kmod $BUILD_DIR/sys/fs/initrd_fs/initrd_fs.o

echo "3. Copying other drivers..."
# Копируем существующие драйверы
if [ -f $MODULES_DIR/drivers/core/vga/vga_driver.kmod ]; then
    cp $MODULES_DIR/drivers/core/vga/vga_driver.kmod $MODULES_DIR/display.kmod
    echo "  ✓ VGA driver"
fi

if [ -f $MODULES_DIR/drivers/core/keyboard/keyboard.kmod ]; then
    cp $MODULES_DIR/drivers/core/keyboard/keyboard.kmod $MODULES_DIR/input.kmod
    echo "  ✓ Keyboard driver"
fi

echo "=== Modules built successfully ==="
ls -la $MODULES_DIR/*.kmod
