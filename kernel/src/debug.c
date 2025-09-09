#include <kernel/debug.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <kernel/printk.h>
#include <kernel/string.h>

static const char* current_test = NULL;

void debug_set_test_name(const char* name) {
    current_test = name;
}

void debug_test_start(const char* name) {
    debug_set_test_name(name);
    printk("Test: %s... ", name);
}

void debug_test_end() {
    printk("OK\n");
    debug_set_test_name(NULL);
}

void debug_assert(int condition, const char* file, int line, const char* format, ...) {
    if (!condition) {
        // Форматируем пользовательское сообщение
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        // Формируем полное сообщение об ошибке
        char full_msg[256];
        snprintf(full_msg, sizeof(full_msg),
                "ASSERTION FAILED in %s at %s:%d: %s",
                current_test ? current_test : "kernel", file, line, buffer);

        printk("%s\n", full_msg);
        PANIC("Assertion failed");
    }
}

void panic(const char *message, const char *file, uint32_t line) {
    printk("PANIC: %s at %s:%d\n", message, file, line);
    asm volatile("cli");
    for (;;) {
        asm volatile("hlt");
    }
}
