#include <kernel/printk.h>
#include <kernel/hal/serial.h>
#include <stdarg.h>

void printk_init(void) {
    // Инициализация уже выполнена в serial_init
}

void printk(const char* format, ...) {
    // Простая реализация - выводим строку через serial
    serial_puts(format);
}

int kprintf(const char* format, ...) {
    // Используем ту же реализацию что и printk
    printk(format);
    return 0;
}

int vsnprintf(char* str, size_t size, const char* format, va_list args) {
    // Упрощенная реализация - просто копируем форматную строку
    size_t i;
    for (i = 0; i < size - 1 && format[i] != '\0'; i++) {
        str[i] = format[i];
    }
    str[i] = '\0';
    return i;
}

int snprintf(char* str, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}
