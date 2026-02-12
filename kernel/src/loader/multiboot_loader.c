#include <kernel/printk.h>

void load_multiboot_modules(unsigned long addr) {
    kprintf("Multiboot modules loading from: 0x%x\n", addr);
    // Временная заглушка - реальная загрузка модулей будет реализована позже
}
