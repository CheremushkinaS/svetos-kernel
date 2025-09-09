#include <kernel/printk.h>

void kernel_main() {
    // Простейшая инициализация serial
    serial_init();
    printk("MINIMAL KERNEL: Hello World!\n");
    
    // Бесконечный цикл
    while (1) {
        __asm__ volatile ("hlt");
    }
}
