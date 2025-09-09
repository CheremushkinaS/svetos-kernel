#include <kernel/module.h>
#include <kernel/printk.h>
#include <kernel/vga.h>

int vga_init(void) {
    printk("VGA module initialized\n");
    return 0;
}

int vga_deinit(void) {
    printk("VGA module deinitialized\n");
    return 0;
}
