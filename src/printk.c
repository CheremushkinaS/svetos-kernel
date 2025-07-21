#include <kernel/printk.h>
#include <stdarg.h>
#include <stdint.h>

static int cursor_pos = 160; // Начало второй строки

void scroll_screen() {
    volatile char *video = (volatile char*)0xB8000;
    // Прокрутка на 1 строку вверх
    for (int i = 0; i < 24*80; i++) {
        video[i*2] = video[(i+80)*2];
        video[i*2+1] = video[(i+80)*2+1];
    }
    // Очистка последней строки
    for (int i = 24*80; i < 25*80; i++) {
        video[i*2] = ' ';
        video[i*2+1] = 0x0F;
    }
    cursor_pos -= 160;
}

void print_char(char c, uint8_t color) {
    volatile char *video = (volatile char*)0xB8000;
    
    if (c == '\n') {
        cursor_pos = ((cursor_pos / 160) + 1) * 160;
        if (cursor_pos >= 25 * 160) scroll_screen();
        return;
    }
    
    video[cursor_pos++] = c;
    video[cursor_pos++] = color;
    
    if (cursor_pos >= 25 * 160) scroll_screen();
}

void printk(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    for (const char *p = format; *p; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'x': {
                    uint32_t val = va_arg(args, uint32_t);
                    char hex_chars[] = "0123456789ABCDEF";
                    for (int i = 7; i >= 0; i--) {
                        uint8_t nibble = (val >> (i * 4)) & 0xF;
                        print_char(hex_chars[nibble], 0x0F);
                    }
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    if (val < 0) {
                        print_char('-', 0x0F);
                        val = -val;
                    }
                    char buf[12];
                    char *ptr = buf;
                    do {
                        *ptr++ = '0' + (val % 10);
                        val /= 10;
                    } while (val > 0);
                    while (ptr > buf) {
                        ptr--;
                        print_char(*ptr, 0x0F);
                    }
                    break;
                }
                case 'u': {
                    uint32_t val = va_arg(args, uint32_t);
                    char buf[12];
                    char *ptr = buf;
                    do {
                        *ptr++ = '0' + (val % 10);
                        val /= 10;
                    } while (val > 0);
                    while (ptr > buf) {
                        ptr--;
                        print_char(*ptr, 0x0F);
                    }
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    while (*str) print_char(*str++, 0x0F);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    print_char(c, 0x0F);
                    break;
                }
                default:
                    print_char('%', 0x0F);
                    print_char(*p, 0x0F);
            }
        } else {
            print_char(*p, 0x0F);
        }
    }
    
    va_end(args);
}

void early_printk(const char* message) {
    volatile char *video = (volatile char*)0xB8000 + 160;
    for (int i = 0; message[i]; i++) {
        video[i*2] = message[i];
        video[i*2+1] = 0x0F;
    }
}
