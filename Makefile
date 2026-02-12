# Компиляторы и утилиты
CC = gcc
ASM = nasm
LD = ld
GRUB_MKRESCUE = grub-mkrescue

# Флаги оптимизации
OPTIMIZATION_FLAGS = -O2 -fno-strict-aliasing -funroll-loops -ftree-vectorize
SECURITY_FLAGS =
PERFORMANCE_FLAGS = -march=native -mtune=native

CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra \
         -Iinclude -Iinclude/kernel -Iinclude/std \
         -m32 -nostdinc -fno-builtin \
         $(OPTIMIZATION_FLAGS) $(SECURITY_FLAGS) $(PERFORMANCE_FLAGS)

# Флаги для модулей
MODULE_CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra \
                -Iinclude -Iinclude/kernel -Iinclude/std \
                -m32 -nostdinc -fno-builtin -O2

MODULE_LDFLAGS = -nostdlib -m elf_i386

# Целевые платформы
ifeq ($(TARGET), mobile)
    CFLAGS += -DMOBILE_OPTIMIZATIONS -DENERGY_EFFICIENT
else ifeq ($(TARGET), embedded)
    CFLAGS += -DEMBEDDED_OPTIMIZATIONS -DSMALL_MEMORY
else
    CFLAGS += -DDESKTOP_OPTIMIZATIONS -DPERFORMANCE_ORIENTED
endif

# Сборка с аппаратным ускорением если available
ifdef CRYPTO_ACCELERATOR
    CFLAGS += -DCRYPTO_ACCELERATOR_AVAILABLE
endif

ifdef IPC_ACCELERATOR
    CFLAGS += -DIPC_ACCELERATOR_AVAILABLE
endif

ASMFLAGS = -f elf32
LDFLAGS = -T kernel/linker.ld -nostdlib -m elf_i386

# Основные исходные файлы
C_SOURCES = $(shell find kernel/src -name "*.c" ! -name "minimal_kernel.c")
ASM_SOURCES = $(shell find kernel/src -name "*.asm" ! -name "gdt_asm.asm")
OBJS = $(sort $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o))

# Модули
MODULE_SOURCES = \
	modules/drivers/storage/ata/ata_module_simple.c \
	modules/sys/fs/initrd_fs/initrd_fs_module_simple.c

MODULE_OBJS = $(MODULE_SOURCES:.c=.o)
MODULE_TARGETS = \
	modules/ata.kmod \
	modules/initrd_fs.kmod

# Проверка на дубликаты
DUPLICATES = $(shell echo $(OBJS) | tr " " "\n" | sort | uniq -d)
ifneq ($(DUPLICATES),)
$(error Found duplicate object files: $(DUPLICATES))
endif

# Цели по умолчанию
all: build/kernel.bin initrd.img modules

# Сборка ядра
build/kernel.bin: $(OBJS) kernel/linker.ld
	@mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Компиляция C файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ассемблирование ASM файлов
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Сборка модулей
modules: $(MODULE_TARGETS)

modules/ata.kmod: modules/drivers/storage/ata/ata_module.o
	@mkdir -p modules
	$(LD) $(MODULE_LDFLAGS) -o $@ $^

modules/initrd_fs.kmod: modules/sys/fs/initrd_fs/initrd_fs_module.o
	@mkdir -p modules
	$(LD) $(MODULE_LDFLAGS) -o $@ $^

# Компиляция модулей
modules/%.o: modules/%.c
	@mkdir -p $(dir $@)
	$(CC) $(MODULE_CFLAGS) -c $< -o $@

# Создание ISO образа
iso: build/kernel.bin initrd.img modules
	@mkdir -p iso/boot/grub
	@mkdir -p iso/boot/modules
	@cp build/kernel.bin iso/boot/
	@cp initrd.img iso/boot/
	@cp $(MODULE_TARGETS) iso/boot/modules/ 2>/dev/null || true
	@# Копируем дополнительные драйверы если есть
	@if [ -f modules/drivers/core/vga/vga_driver.kmod ]; then \
		cp modules/drivers/core/vga/vga_driver.kmod iso/boot/modules/display.kmod; \
	fi
	@if [ -f modules/drivers/core/keyboard/keyboard.kmod ]; then \
		cp modules/drivers/core/keyboard/keyboard.kmod iso/boot/modules/input.kmod; \
	fi
	@cp grub-modular.cfg iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o svetos-modular.iso iso
	@echo "=== Modular ISO created: svetos-modular.iso ==="
	@echo "Modules included:"
	@ls -la iso/boot/modules/

# Создание initrd образа
initrd.img: tools/create_initrd.sh
	./tools/create_initrd.sh

# Очистка
clean:
	rm -rf build iso svetos.iso svetos-modular.iso initrd.img initrd_root
	find . -name "*.o" -delete
	find modules -name "*.kmod" -delete

# Запуск в QEMU через ISO
run: iso
	qemu-system-i386 -cdrom svetos-modular.iso -nographic -serial stdio -monitor none

# Расширенные команды QEMU для разработки
run-gui: iso
	qemu-system-i386 -enable-kvm -m 2G -cdrom svetos-modular.iso

run-debug: iso
	qemu-system-i386 -s -S -cdrom svetos-modular.iso -nographic -serial stdio

run-headless: iso
	qemu-system-i386 -nographic -serial mon:stdio -cdrom svetos-modular.iso

# Отладка с GDB
debug: build/kernel.bin
	gdb build/kernel.bin

# Специальные цели для разработки модулей
modules-clean:
	find modules -name "*.o" -delete
	find modules -name "*.kmod" -delete

modules-list:
	@echo "Available modules:"
	@find modules -name "*.kmod" | while read file; do \
		echo "  - $$file"; \
	done

# Проверка размера ядра
kernel-size: build/kernel.bin
	@size=$$(stat -c%s build/kernel.bin); \
	echo "Kernel size: $$size bytes"; \
	if [ $$size -gt 102400 ]; then \
		echo "WARNING: Kernel exceeds 100KB limit!"; \
	else \
		echo "Kernel size is within 100KB limit"; \
	fi

.PHONY: all iso clean run run-gui run-debug run-headless debug modules modules-clean modules-list kernel-size

# Компилируем заглушки для модулей
modules/module_stubs.o: modules/module_stubs.c
	@mkdir -p modules
	$(CC) $(MODULE_CFLAGS) -c $< -o $@

# Обновляем правила линковки модулей с заглушками
modules/ata.kmod: modules/drivers/storage/ata/ata_module.o modules/module_stubs.o
	@mkdir -p modules
	$(LD) $(MODULE_LDFLAGS) -o $@ $^

modules/initrd_fs.kmod: modules/sys/fs/initrd_fs/initrd_fs_module.o modules/module_stubs.o
	@mkdir -p modules
	$(LD) $(MODULE_LDFLAGS) -o $@ $^
