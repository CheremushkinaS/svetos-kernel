#ifndef PRINTK_H
#define PRINTK_H

#include <stdarg.h>
#include <stddef.h>

void printk_init(void);
void printk(const char* format, ...);
int vsnprintf(char* str, size_t size, const char* format, va_list args);
int snprintf(char* str, size_t size, const char* format, ...);

#endif
