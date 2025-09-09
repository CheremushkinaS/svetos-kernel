#include <kernel/module.h>
#include <kernel/printk.h>

int timer_init(void) {
    printk("Timer module initialized\n");
    return 0;
}

int timer_deinit(void) {
    printk("Timer module deinitialized\n");
    return 0;
}
