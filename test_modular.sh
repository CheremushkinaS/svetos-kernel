#!/bin/bash

echo "=== Testing SvetOS Modular System ==="

# Очистка
echo "1. Cleaning..."
make clean

# Сборка ядра
echo "2. Building kernel..."
make build/kernel.bin

# Проверка размера
echo "3. Checking kernel size..."
make kernel-size

# Сборка модулей
echo "4. Building modules..."
make modules

# Создание ISO
echo "5. Creating modular ISO..."
make iso

echo "=== Build Complete ==="
echo "ISO: svetos-modular.iso"
echo ""
echo "To test: make run"
echo "Or: qemu-system-i386 -cdrom svetos-modular.iso -m 512 -serial stdio"
