#!/bin/bash

echo "Creating SVETOS ISO image..."

# Clean previous build
rm -rf iso
rm -f svetos.iso

# Create directory structure for ISO
mkdir -p iso/boot/grub

# Copy kernel and initrd to ISO directory
cp build/kernel.bin iso/boot/
cp initrd.img iso/boot/

# Copy GRUB configuration
cp grub.cfg iso/boot/grub/

echo "Creating ISO with GRUB..."
grub-mkrescue -o svetos.iso iso

if [ $? -eq 0 ]; then
    echo "ISO image created successfully: svetos.iso"
    ls -la svetos.iso
else
    echo "Failed to create ISO image"
    exit 1
fi
