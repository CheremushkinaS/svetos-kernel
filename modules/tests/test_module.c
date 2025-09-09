#include "module.h"
#include "printk.h"

int test_init(void) {
    printk("Test module initialized\n");
    return 0;
}

int test_deinit(void) {
    printk("Test module deinitialized\n");
    return 0;
}

int test_run_tests(void) {
    printk("Running test module tests\n");
    return 0;
}

// Временное решение: создаем структуру модуля вручную
// вместо использования макроса MODULE_DEF
module_t test_module = {
    .name = "test_module",
    .init = test_init,
    .deinit = test_deinit,
    .run_tests = test_run_tests
};
