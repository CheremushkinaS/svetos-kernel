#include <kernel/panic.h>
#include <kernel/printk.h>

void kernel_panic(const char* message) {
    printk("KERNEL PANIC: %s\n", message);
    printk("System halted\n");

    __asm__ volatile ("cli");
    while (1) {
        __asm__ volatile ("hlt");
    }
}
