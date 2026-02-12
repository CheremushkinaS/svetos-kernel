#include <stddef.h>
#include <stddef.h>
#include <stddef.h>
// Заглушки для функций ядра, которые будут разрешены при загрузке
// Эти функции предоставляются ядром

void printk(const char* fmt, ...) {
    // Эта функция будет заменена на реальную при загрузке модуля
    // Временно ничего не делаем
    (void)fmt;
}

void interface_register(const char* name, void* interface) {
    // Заглушка, будет заменена при загрузке
    (void)name;
    (void)interface;
}

void* interface_get(const char* name) {
    // Заглушка
    (void)name;
    return NULL;
}

void* kmalloc(size_t size) {
    // Заглушка для выделения памяти
    (void)size;
    return NULL;
}

void kfree(void* ptr) {
    // Заглушка для освобождения памяти
    (void)ptr;
}

// Простые реализации строковых функций
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    while (n--) *d++ = *s++;
    return dest;
}
