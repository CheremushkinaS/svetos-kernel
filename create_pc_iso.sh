#!/bin/bash

echo "=== Building SvetOS PC Version ==="

# Создаем структуру директорий
mkdir -p iso/boot/grub
mkdir -p iso/boot/modules

echo "Copying kernel..."
cp build/kernel.bin iso/boot/

echo "Copying initrd..."
cp initrd.img iso/boot/

echo "Installing PC modules..."
# Копируем ATA драйвер как основной драйвер диска
if [ -f modules/ata.kmod ]; then
    cp modules/ata.kmod iso/boot/modules/block_storage.kmod
    echo "  ✓ ATA driver -> block_storage.kmod"
else
    echo "  ✗ ATA driver not found!"
    exit 1
fi

# Копируем файловую систему
if [ -f modules/initrd_fs.kmod ]; then
    cp modules/initrd_fs.kmod iso/boot/modules/filesystem.kmod
    echo "  ✓ InitRD FS -> filesystem.kmod"
else
    echo "  ✗ InitRD FS not found!"
    exit 1
fi

# Копируем дополнительные драйверы если есть
if [ -f modules/drivers/core/vga/vga_driver.kmod ]; then
    cp modules/drivers/core/vga/vga_driver.kmod iso/boot/modules/display.kmod
    echo "  ✓ VGA driver -> display.kmod"
fi

if [ -f modules/drivers/core/keyboard/keyboard.kmod ]; then
    cp modules/drivers/core/keyboard/keyboard.kmod iso/boot/modules/input.kmod
    echo "  ✓ Keyboard driver -> input.kmod"
fi

echo "Creating GRUB configuration..."
cat > iso/boot/grub/grub.cfg << 'GRUB_EOF'
menuentry "SvetOS PC" {
    multiboot2 /boot/kernel.bin
    module2 /boot/initrd.img
    module2 /boot/modules/block_storage.kmod
    module2 /boot/modules/filesystem.kmod
    module2 /boot/modules/display.kmod
    module2 /boot/modules/input.kmod
}

menuentry "SvetOS PC (Minimal)" {
    multiboot2 /boot/kernel.bin
    module2 /boot/initrd.img
    module2 /boot/modules/block_storage.kmod
    module2 /boot/modules/filesystem.kmod
}
GRUB_EOF

echo "Creating ISO..."
grub-mkrescue -o svetos-pc.iso iso/

echo "=== PC ISO created: svetos-pc.iso ==="
echo "Modules included:"
echo "  - block_storage.kmod (ATA driver)"
echo "  - filesystem.kmod (InitRD filesystem)"
echo "  - display.kmod (VGA driver)"
echo "  - input.kmod (Keyboard driver)"
