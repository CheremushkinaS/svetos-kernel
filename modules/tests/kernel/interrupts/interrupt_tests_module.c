#include <kernel/module.h>
#include <kernel/printk.h>

int interrupt_tests_module_init(void) {
    printk("Interrupt tests module initialized (stub)\n");
    return 0;
}
