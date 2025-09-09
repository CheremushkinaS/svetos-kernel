#include <kernel/module.h>
#include <kernel/printk.h>
#include <kernel/ipc.h>

int keyboard_init(void) {
    printk("Keyboard module initialized\n");
    return 0;
}

int keyboard_deinit(void) {
    printk("Keyboard module deinitialized\n");
    return 0;
}
