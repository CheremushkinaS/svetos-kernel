CC = gcc
LD = ld
AS = nasm
GRUB_MKRESCUE = grub-mkrescue

CFLAGS = -m32 -nostdlib -ffreestanding -O2 -Wall -Wextra -Iinclude
LDFLAGS = -m elf_i386 -T src/linker.ld -nostdlib -z noexecstack
ASFLAGS = -f elf32

KERNEL = build/kernel.bin
ISO = build/svetos.iso
ISO_DIR = iso

# Добавляем все модули памяти
OBJS = build/boot.o build/kernel.o build/printk.o build/mm/mm_dump.o

all: $(ISO)

$(ISO): $(KERNEL)
	cp $(KERNEL) $(ISO_DIR)/boot/
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

build/%.o: src/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

# Правило для сборки модулей памяти
build/mm/%.o: src/mm/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)
	rm -rf build/mm

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -display curses
