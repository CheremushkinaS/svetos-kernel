#include <kernel/modules/module.h>
#include <kernel/printk.h>

// Удаляем неиспользуемую функцию terminal_callback

int init_module(void) {
    printk("Terminal module initialized\n");
    return 0;
}

void deinit_module(void) {
    printk("Terminal module deinitialized\n");
}
