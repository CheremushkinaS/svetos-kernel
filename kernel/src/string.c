#include <kernel/string.h>

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memset(void* s, int c, size_t n) {
    char* p = (char*)s;
    while (n--) {
        *p++ = (char)c;
    }
    return s;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

size_t strlen(const char* s) {
    size_t len = 0;
    while (*s++) len++;
    return len;
}
