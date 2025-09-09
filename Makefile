# Компиляторы и утилиты
CC = gcc
ASM = nasm
LD = ld
GRUB_MKRESCUE = grub-mkrescue

# Флаги компиляции
CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -Iinclude -Iinclude/kernel -Iinclude/std -Ikernel/include -Ikernel/include/multiboot -Ikernel/include/hal -m32 -nostdinc -fno-builtin
ASMFLAGS = -f elf32
LDFLAGS = -T kernel/linker.ld -nostdlib -m elf_i386

# Исходные файлы
SRC_C = $(shell find kernel/src -name '*.c' ! -name '*backup*' ! -name 'minimal_kernel.c')
SRC_ASM = $(shell find kernel/src -name '*.asm' ! -name 'gdt_asm.asm')
OBJS = $(sort $(SRC_C:kernel/src/%.c=build/kernel/src/%.o) \
             $(SRC_ASM:kernel/src/%.asm=build/kernel/src/%.o))

# Цели по умолчанию
all: build/kernel.bin

# Сборка ядра
build/kernel.bin: $(OBJS) kernel/linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# Компиляция C файлов
build/kernel/src/%.o: kernel/src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Ассемблирование ASM файлов
build/kernel/src/%.o: kernel/src/%.asm
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $< -o $@

# Создание ISO образа
iso: build/kernel.bin initrd.img
	@mkdir -p iso/boot/grub
	cp build/kernel.bin iso/boot/
	cp initrd.img iso/boot/
	cp grub.cfg iso/boot/grub/
	$(GRUB_MKRESCUE) -o svetos.iso iso

# Создание initrd образа
initrd.img:
	mkdir -p initrd_root
	echo "Test module content" > initrd_root/test.kmod
	echo "VFS core module" > initrd_root/vfs_core.kmod
	cd initrd_root && find . | cpio -o -H newc > ../initrd.img

# Очистка
clean:
	rm -rf build iso svetos.iso initrd.img initrd_root

# Запуск в QEMU (текстовый режим) с автоматическим продолжением
run: iso
	qemu-system-i386 -cdrom svetos.iso -nographic -serial stdio -monitor none

# Запуск в QEMU с выводом в терминал
run-terminal: iso
	qemu-system-i386 -cdrom svetos.iso -nographic -serial stdio -monitor none

# Просмотр логов
view-log:
	cat serial.log

.PHONY: all iso clean run run-terminal view-log initrd.img
