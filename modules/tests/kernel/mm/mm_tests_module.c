#include <kernel/module.h>
#include <kernel/printk.h>

int mm_tests_module_init(void) {
    printk("Memory management tests module initialized (stub)\n");
    return 0;
}
