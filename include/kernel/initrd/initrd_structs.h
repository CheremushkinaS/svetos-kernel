#ifndef INITRD_STRUCTS_H
#define INITRD_STRUCTS_H

#include <kernel/types.h>

// Структура для представления файла в initrd
typedef struct initrd_file {
    char name[256];      // Имя файла
    void* data;          // Указатель на данные файла
    uint32_t size;       // Размер файла
    uint32_t index;      // Индекс файла
} initrd_file_t;

// Функции для работы с initrd
initrd_file_t* initrd_get_files(void);
uint32_t initrd_get_file_count(void);
initrd_file_t* initrd_find_file(const char* name);

#endif // INITRD_STRUCTS_H
