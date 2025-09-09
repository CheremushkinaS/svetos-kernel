#include <kernel/module.h>
#include <kernel/printk.h>

int vga_driver_module_init(void) {
    printk("VGA driver module initialized (stub)\n");
    return 0;
}
