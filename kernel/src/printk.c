#include <kernel/printk.h>
#include <kernel/tty.h>
#include <hal/serial.h>
#include <stdarg.h>
#include <kernel/string.h>
#include <stdint.h>

// Вспомогательная функция для вывода строк в serial с проверкой готовности
static void serial_print(const char* str) {
    while (*str) {
        // Проверяем готовность serial порта перед отправкой каждого символа
        int timeout = 1000;
        while (!serial_is_ready() && timeout--) {
            // Короткая задержка
            for (volatile int j = 0; j < 100; j++) {}
        }

        if (timeout > 0) {
            serial_putc(*str++);
        } else {
            // Если порт не готов, пропускаем символ
            str++;
        }
    }
}

void printk_init(void) {
    serial_init_com1(); // Используем новую функцию без параметров
    tty_init();
}

int vsnprintf(char* str, size_t size, const char* format, va_list args) {
    if (!str || size == 0) return 0;

    char* ptr = str;
    char* end = str + size - 1;

    while (*format && ptr < end) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    if (num < 0) {
                        *ptr++ = '-';
                        num = -num;
                    }

                    char buffer[32];
                    char* p = buffer;
                    do {
                        *p++ = '0' + (num % 10);
                        num /= 10;
                    } while (num > 0);

                    while (p > buffer && ptr < end) {
                        *ptr++ = *--p;
                    }
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    const char* hex = "0123456789abcdef";

                    char buffer[32];
                    char* p = buffer;
                    do {
                        *p++ = hex[num & 0xF];
                        num >>= 4;
                    } while (num > 0);

                    if (ptr + 2 < end) {
                        *ptr++ = '0';
                        *ptr++ = 'x';
                    }

                    while (p > buffer && ptr < end) {
                        *ptr++ = *--p;
                    }
                    break;
                }
                case 's': {
                    const char* s = va_arg(args, const char*);
                    while (*s && ptr < end) {
                        *ptr++ = *s++;
                    }
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    if (ptr < end) *ptr++ = c;
                    break;
                }
                case '%': {
                    if (ptr < end) *ptr++ = '%';
                    break;
                }
                default:
                    if (ptr < end) *ptr++ = '%';
                    if (ptr < end) *ptr++ = *format;
                    break;
            }
            format++;
        } else {
            *ptr++ = *format++;
        }
    }

    *ptr = '\0';
    return ptr - str;
}

int snprintf(char* str, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}

void printk(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Вывод через serial с защитой от задержек
    serial_print(buffer);

    // Также выводим на экран через TTY
    terminal_write(buffer, strlen(buffer));

    va_end(args);
}
