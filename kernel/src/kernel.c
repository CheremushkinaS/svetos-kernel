#include <kernel/kernel.h>
#include <kernel/debug.h>
#include <kernel/gdt.h>
#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>
#include <kernel/interrupts/irq.h>
#include <kernel/hal/serial.h>
#include <kernel/printk.h>
#include <kernel/mm/mm.h>
#include <multiboot.h>

// Временные заглушки для отсутствующих функций
void ipc_init(void) {
    kprintf("IPC stub\n");
}

void load_multiboot_modules(unsigned long addr) {
    kprintf("Multiboot modules loader stub - addr: 0x%x\n", addr);
}

void initialize_modules(void) {
    kprintf("Module manager stub\n");
}

void kernel_main(unsigned long mbi_addr) {
    // Инициализация основных систем
    serial_init();
    kprintf("SvetOS kernel starting...\n");

    gdt_init();
    kprintf("GDT initialized\n");

    idt_init();
    kprintf("IDT initialized\n");

    pic_remap(0x20, 0x28);
    kprintf("PIC remapped\n");

    irq_init();
    kprintf("IRQ initialized\n");

    kprintf("Interrupts initialized\n");

    // Инициализация памяти
    mm_init((multiboot_info_t*)mbi_addr);
    kprintf("Memory manager initialized\n");

    // Инициализация IPC
    ipc_init();
    kprintf("IPC initialized\n");

    // Загрузка модулей multiboot
    load_multiboot_modules(mbi_addr);
    kprintf("Multiboot modules loaded\n");

    // Инициализация модулей ядра
    initialize_modules();
    kprintf("Modules initialized\n");

    kprintf("Kernel initialized successfully\n");

    // Основной цикл ядра
    while (1) {
        asm volatile ("hlt");
    }
}
