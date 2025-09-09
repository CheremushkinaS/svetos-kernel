#include <kernel/string.h>
#include <stdbool.h>

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char* ptr = str;
    bool negative = false;

    if (value < 0 && base == 10) {
        negative = true;
        value = -value;
    }

    // Сначала преобразуем в обратном порядке
    do {
        int remainder = value % base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder];
        value /= base;
    } while (value);

    if (negative) {
        *ptr++ = '-';
    }

    *ptr = '\0';

    // Перевернем строку
    char* start = str;
    char* end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }

    return str;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if (n == (size_t)-1 || *s1 == *s2) {
        return 0;
    }
    return (*(unsigned char*)s1 - *(unsigned char*)s2);
}

// Реализация strstr
char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;

    for (const char* h = haystack; *h; h++) {
        const char* n = needle;
        const char* h2 = h;

        while (*h2 && *n && *h2 == *n) {
            h2++;
            n++;
        }

        if (!*n) return (char*)h;
    }

    return NULL;
}

// Реализация strrchr
char* strrchr(const char* s, int c) {
    char* last = NULL;

    while (*s) {
        if (*s == (char)c) {
            last = (char*)s;
        }
        s++;
    }

    return last;
}

// Реализация strcat
char* strcat(char* dest, const char* src) {
    char* ptr = dest;

    while (*ptr) ptr++;

    while (*src) {
        *ptr++ = *src++;
    }

    *ptr = '\0';
    return dest;
}

// Реализация memcmp
int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = s1;
    const unsigned char* p2 = s2;

    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }

    return 0;
}

// Реализация strchr
char* strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c) {
            return (char*)s;
        }
        s++;
    }
    return NULL;
}

// Реализация strtok
char* strtok(char* str, const char* delim) {
    static char* last = NULL;
    char* token;
    
    if (str) {
        last = str;
    }
    
    if (!last || !*last) {
        return NULL;
    }
    
    // Пропускаем разделители в начале
    while (*last && strchr(delim, *last)) {
        last++;
    }
    
    if (!*last) {
        return NULL;
    }
    
    token = last;
    
    // Ищем конец токена
    while (*last && !strchr(delim, *last)) {
        last++;
    }
    
    if (*last) {
        *last++ = '\0';
    }
    
    return token;
}
