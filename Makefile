CC = gcc
LD = ld
AS = nasm
GRUB_MKRESCUE = grub-mkrescue

CFLAGS = -m32 -nostdlib -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386 -T src/linker.ld -nostdlib
ASFLAGS = -f elf32

KERNEL = build/kernel.bin
ISO = build/svetos.iso
ISO_DIR = iso

OBJS = build/boot.o build/kernel.o

all: $(ISO)

$(ISO): $(KERNEL)
cp $(KERNEL) $(ISO_DIR)/boot/
$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

$(KERNEL): $(OBJS)
$(LD) $(LDFLAGS) -o $@ $^

build/%.o: src/%.asm
$(AS) $(ASFLAGS) -o $@ $<

build/%.o: src/%.c
$(CC) $(CFLAGS) -c -o $@ $<

clean:
rm -f $(OBJS) $(KERNEL) $(ISO)

run: $(ISO)
qemu-system-i386 -cdrom $(ISO)
