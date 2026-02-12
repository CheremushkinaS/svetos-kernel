#!/bin/bash

echo "=== Building SvetOS Modular ISO ==="

# Создаем структуру директорий
mkdir -p iso/boot/grub
mkdir -p iso/boot/modules

echo "1. Building kernel..."
make clean
make

echo "2. Building modules..."
./build_modules.sh

echo "3. Copying files to ISO..."
cp build/kernel.bin iso/boot/
cp initrd.img iso/boot/

# Копируем модули
cp modules/ata.kmod iso/boot/modules/
cp modules/initrd_fs.kmod iso/boot/modules/

# Копируем дополнительные драйверы если есть
if [ -f modules/display.kmod ]; then
    cp modules/display.kmod iso/boot/modules/
fi

if [ -f modules/input.kmod ]; then
    cp modules/input.kmod iso/boot/modules/
fi

echo "4. Creating GRUB configuration..."
cat > iso/boot/grub/grub.cfg << 'GRUB_EOF'
menuentry "SvetOS Modular with Disk Driver" {
    multiboot2 /boot/kernel.bin
    module2 /boot/initrd.img
    module2 /boot/modules/ata.kmod
    module2 /boot/modules/initrd_fs.kmod
    module2 /boot/modules/display.kmod
    module2 /boot/modules/input.kmod
}

menuentry "SvetOS Minimal (Disk only)" {
    multiboot2 /boot/kernel.bin
    module2 /boot/initrd.img
    module2 /boot/modules/ata.kmod
    module2 /boot/modules/initrd_fs.kmod
}

menuentry "SvetOS Kernel only (no modules)" {
    multiboot2 /boot/kernel.bin
    module2 /boot/initrd.img
}
GRUB_EOF

echo "5. Creating ISO image..."
grub-mkrescue -o svetos-modular.iso iso/

echo "=== Modular ISO created: svetos-modular.iso ==="
echo ""
echo "This version uses REAL module loading:"
echo "  - ATA driver loads as separate module via GRUB"
echo "  - Filesystem loads as separate module via GRUB" 
echo "  - Modules are loaded at boot time and initialized by kernel"
echo ""
echo "Modules included in ISO:"
ls -la iso/boot/modules/
