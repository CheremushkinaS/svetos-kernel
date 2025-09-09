#include <kernel/module.h>
#include <kernel/printk.h>

// Объявления функций инициализации внешних модулей
extern int keyboard_module_init();
extern int timer_module_init();
extern int vga_driver_module_init();
extern int test_module_init();
extern int interrupt_tests_module_init();
extern int mm_tests_module_init();

void register_external_modules(void) {
    printk("External modules registered\n");
    module_register_external("keyboard", "Keyboard Driver", MODULE_TYPE_DRIVER, DRIVER_SUBTYPE_INPUT, 1, 0, keyboard_module_init, NULL, NULL);
    module_register_external("timer", "Timer Driver", MODULE_TYPE_DRIVER, DRIVER_SUBTYPE_TIME, 1, 0, timer_module_init, NULL, NULL);
    module_register_external("vga_driver", "VGA Driver", MODULE_TYPE_DRIVER, DRIVER_SUBTYPE_VIDEO, 1, 0, vga_driver_module_init, NULL, NULL);
    module_register_external("test_module", "Test Module", MODULE_TYPE_SERVICE, DRIVER_SUBTYPE_NONE, 1, 0, test_module_init, NULL, NULL);
    module_register_external("interrupt_tests", "Interrupt Tests", MODULE_TYPE_TEST, DRIVER_SUBTYPE_NONE, 1, 0, interrupt_tests_module_init, NULL, NULL);
    module_register_external("mm_tests", "Memory Management Tests", MODULE_TYPE_TEST, DRIVER_SUBTYPE_NONE, 1, 0, mm_tests_module_init, NULL, NULL);
}
