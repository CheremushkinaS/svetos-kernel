#ifndef STRING_H
#define STRING_H

#include <stddef.h>

// Основные функции работы со строками
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strncmp(const char* s1, const char* s2, size_t n);

// Новые функции
char* strstr(const char* haystack, const char* needle);
char* strrchr(const char* s, int c);
char* strcat(char* dest, const char* src);
char* strtok(char* str, const char* delim);
char* strchr(const char* s, int c);

// Функции работы с памятью
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

// Дополнительные функции
char* itoa(int value, char* str, int base);

#endif
