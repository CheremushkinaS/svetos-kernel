#!/bin/bash

# Create directory structure for ISO
mkdir -p iso/boot/grub

# Copy kernel and initrd to ISO directory
cp build/kernel.bin iso/boot/
cp initrd.img iso/boot/

# Copy GRUB configuration
cp grub.cfg iso/boot/grub/

# Create ISO image
grub-mkrescue -o svetos.iso iso

echo "ISO image created: svetos.iso"
