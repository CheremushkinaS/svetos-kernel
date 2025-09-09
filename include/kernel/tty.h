#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/vga.h>

void tty_init(void);
void terminal_initialize(void);
void terminal_set_color(uint8_t fg, uint8_t bg);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_puts(const char* str);
void terminal_register_handler(void (*handler)(char));
int tty_module_init(void);

#endif
